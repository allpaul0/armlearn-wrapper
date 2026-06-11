#ifndef ARM_LEARNING_AGENT_H
#define ARM_LEARNING_AGENT_H

#include <memory>
#include <queue>
#include <mutex>
#include <thread>
#include <numeric>
#include <chrono>
#include <iostream>

#include <gegelati.h>

#include "../../params/trainingParameters.h"
#include "../evaluation/ArmLearnEvaluationResult.h"
#include "../evaluation/EvaluationPolicy.h"
#include "../environment/TrajectoryManager.h"
#include "CurriculumManager.h"

namespace Learn {

/**
 * @brief Training agent for the WidowX arm.
 *
 * Changes from the original:
 *  - evaluateJob() no longer owns cancellation logic or score aggregation;
 *    both are delegated to IEvaluationPolicy.
 *  - Curriculum updates are delegated to CurriculumManager.
 *  - Logging uses IArmEvaluationResult instead of dynamic_pointer_cast to a
 *    concrete type.
 *  - makeJobs() and trainOneGeneration() are unchanged in structure, but
 *    internal casts have been replaced with interface calls.
 */
class ArmLearningAgent : public ParallelLearningAgent {
public:

    ArmLearningAgent(LearningEnvironment&       le,
                     const Instructions::Set&   iSet,
                     const LearningParameters&  p,
                     TrainingParameters&        trainingParams,
                     IEvaluationPolicy&         evalPolicy,
                     CurriculumManager*         curriculum = nullptr,
                     const TPG::TPGFactory&     factory = TPG::TPGFactory())
        : ParallelLearningAgent(le, iSet, p, factory),
          trainingParams(trainingParams),
          evalPolicy(evalPolicy),
          curriculum(curriculum),
          doTrainingValidation(trainingParams.doTrainingValidation && curriculum != nullptr)
    {}

    // -----------------------------------------------------------------------
    // ParallelLearningAgent overrides
    // -----------------------------------------------------------------------
    void trainOneGeneration(uint64_t generationNumber) override;

    std::shared_ptr<EvaluationResult> evaluateJob(
        TPG::TPGExecutionEngine& tee, const Job& job,
        uint64_t generationNumber, LearningMode mode,
        LearningEnvironment& le) const override;

    std::queue<std::shared_ptr<Job>> makeJobs(
        LearningMode mode, TPG::TPGGraph* tpgGraph = nullptr) override;

    // -----------------------------------------------------------------------
    // Testing helper
    // -----------------------------------------------------------------------
    void testingBestRoot(uint64_t generationNumber);

private:

    TrainingParameters& trainingParams;
    IEvaluationPolicy&  evalPolicy;
    CurriculumManager*  curriculum;       ///< Nullable — no curriculum when null
    bool                doTrainingValidation;

    std::multimap<std::shared_ptr<EvaluationResult>, const TPG::TPGVertex*> bestTrainingResult;
    std::vector<double> fiveLastBest;

    // Helper: downcast to IArmEvaluationResult safely
    static const IArmEvaluationResult& armResult(const std::shared_ptr<EvaluationResult>& r) {
        const auto* arm = dynamic_cast<const IArmEvaluationResult*>(r.get());
        if (!arm) throw std::runtime_error("[ArmLearningAgent] EvaluationResult does not implement IArmEvaluationResult.");
        return *arm;
    }
};

// ---------------------------------------------------------------------------
// Implementation
// ---------------------------------------------------------------------------

inline void ArmLearningAgent::trainOneGeneration(uint64_t gen) {

    std::cout << "logGenStarted" << std::endl;

    for (auto& lg : loggers) lg.get().logNewGeneration(gen);
    
    std::cout << "logGenEnded" << std::endl;

    Mutator::TPGMutator::populateTPG(*tpg, archive, params.mutation, rng, maxNbThreads);

    std::cout << "populateTPG" << std::endl;

    for (auto& lg : loggers) lg.get().logAfterPopulateTPG();

    std::cout << "logAfterPopulateTPG" << std::endl;

    // ---- Training evaluation ----
    auto results = evaluateAllRoots(gen, LearningMode::TRAINING);
    for (auto& lg : loggers) lg.get().logAfterEvaluate(results);

    std::cout << "evaluateAllRoots" << std::endl;

    // Track five-last best
    auto itBest = std::prev(results.end());
    const double bestScore = itBest->first->getResult();
    fiveLastBest.push_back(bestScore);
    if (gen >= 5) fiveLastBest.erase(fiveLastBest.begin());

    // Forward trajectory scores back to the environment
    for (const auto& [idx, sc] : dynamic_cast<const ArmLearnEvaluationResult*>(itBest->first.get())->getTrajScores()) {
        dynamic_cast<ArmLearnWrapper&>(learningEnvironment).addToScoreTrajectories(idx, sc);
    }

    decimateWorstRoots(results);

    // Keep top-5 training results for validation job selection
    bestTrainingResult.clear();
    auto it = results.rbegin();
    for (int i = 0; i < 5 && it != results.rend(); ++i, ++it)
        bestTrainingResult.insert(*it);

    // ---- Validation ----
    if (params.doValidation) {
        auto valResults = evaluateAllRoots(gen, LearningMode::VALIDATION);
        for (auto& lg : loggers) lg.get().logAfterValidate(valResults);
        updateEvaluationRecords(valResults);
    } else {
        updateEvaluationRecords(results);
    }

    // ---- Training-validation (curriculum) ----
    double bestDistance = 0.0;
    if (doTrainingValidation) {
        auto tvResults = evaluateAllRoots(gen, LearningMode::TESTING);
        for (auto& lg : loggers) lg.get().logAfterValidate(tvResults); // reuse same hook

        auto itTv = std::prev(tvResults.end());
        bestDistance = armResult(itTv->first).getDistance();
    }

    // ---- Curriculum update ----
    if (curriculum && curriculum->isActive()) {
        curriculum->update(bestDistance, params.nbIterationsPerPolicyEvaluation);
    }

    for (auto& lg : loggers) lg.get().logEndOfTraining();
}

inline std::shared_ptr<EvaluationResult> ArmLearningAgent::evaluateJob(
    TPG::TPGExecutionEngine& tee, const Job& job,
    uint64_t gen, LearningMode mode, LearningEnvironment& le) const
{
    const TPG::TPGVertex* root = job.getRoot();

    std::shared_ptr<EvaluationResult> previousEval;
    if (mode == LearningMode::TRAINING && isRootEvalSkipped(*root, previousEval))
        return previousEval;

    const uint64_t nbIter = (mode == LearningMode::TRAINING)
        ? trainingParams.nbIterationTraining
        : params.nbIterationsPerPolicyEvaluation;

    double totalScore    = 0.0;
    double totalDistance = 0.0;
    double totalSuccess  = 0.0;

    std::vector<double>                  scores;
    std::vector<std::pair<int, double>>  trajScores;
    scores.reserve(nbIter);

    const double refBest = fiveLastBest.empty() ? 0.0
        : std::accumulate(fiveLastBest.begin(), fiveLastBest.end(), 0.0) / fiveLastBest.size();

    bool     cancelled  = false;
    uint64_t actualIter = nbIter;

    for (uint64_t iter = 0; iter < nbIter && !cancelled; ++iter) {

        if (trainingParams.testing)
            std::cout << "Episode " << iter + 1 << "/" << nbIter << "      \r" << std::flush;

        Data::Hash<uint64_t> hasher;
        le.reset(hasher(gen) ^ hasher(iter), mode);

        uint64_t nbActions = 0;
        while (!le.isTerminal() && nbActions < params.maxNbActionsPerEval) {
            uint64_t actionID =
                ((const TPG::TPGAction*)tee.executeFromRoot(*root).first.back())->getActionID();
            le.doAction(actionID);
            ++nbActions;
        }

        const double s = le.getScore();
        const double d = static_cast<ArmLearnWrapper&>(le).getDistance();

        totalScore    += s;
        totalDistance += d;
        totalSuccess  += (d < trainingParams.rangeTarget) ? 1.0 : 0.0;
        scores.push_back(s);
        trajScores.push_back({static_cast<int>(iter), s});

        if (mode == LearningMode::TRAINING) {
            cancelled = evalPolicy.shouldCancel(scores, gen, refBest);
            if (cancelled) actualIter = iter + 1;
        }
    }

    if (trainingParams.testing)
        static_cast<ArmLearnWrapper&>(le).logTestingTrajectories(true, "outLogs");

    const double meanScore = evalPolicy.aggregate(scores);

    auto result = std::make_shared<ArmLearnEvaluationResult>(
        meanScore,
        totalSuccess  / static_cast<double>(actualIter),
        totalDistance / static_cast<double>(actualIter),
        std::move(trajScores),
        actualIter);

    if (previousEval) *result += *previousEval;
    return result;
}

inline std::queue<std::shared_ptr<Job>> ArmLearningAgent::makeJobs(
    LearningMode mode, TPG::TPGGraph* tpgGraph)
{
    tpgGraph = tpgGraph ? tpgGraph : tpg.get();
    std::queue<std::shared_ptr<Job>> jobs;

    if (mode == LearningMode::TRAINING) {
        for (const auto* root : tpgGraph->getRootVertices())
            jobs.push(makeJob(root, mode, static_cast<int>(jobs.size())));
    } else {
        int idx = 0;
        for (const auto& [res, root] : bestTrainingResult)
            jobs.push(makeJob(root, mode, idx++));
    }
    return jobs;
}

inline void ArmLearningAgent::testingBestRoot(uint64_t gen) {
    auto tee  = tpg->getFactory().createTPGExecutionEngine(env, nullptr);
    auto root = tpg->getRootVertices().at(0);
    auto job  = makeJob(root, LearningMode::VALIDATION);

    archive.setRandomSeed(job->getArchiveSeed());

    const auto t0     = std::chrono::high_resolution_clock::now();
    auto result       = evaluateJob(*tee, *job, gen, LearningMode::VALIDATION, learningEnvironment);
    const auto us     = std::chrono::duration_cast<std::chrono::microseconds>(
                            std::chrono::high_resolution_clock::now() - t0).count();

    std::cout << "Testing score: " << result->getResult()
              << "  success: " << armResult(result).getSuccess()
              << "  duration: " << us / 1e6 << "s (" << us << " µs over " << gen << " gen)\n";
}

} // namespace Learn

#endif // ARM_LEARNING_AGENT_H
