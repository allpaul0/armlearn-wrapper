#ifndef ARM_LEARN_LOGGER_H
#define ARM_LEARN_LOGGER_H

#include <ostream>
#include <iostream>
#include <iomanip>
#include <numeric>

#include <gegelati.h>

#include "ITrainingLogger.h"
#include "../evaluation/IArmEvaluationResult.h"

namespace Log {

/**
 * @brief Tabular logger for the arm training loop.
 *
 * Inherits from both LALogger (required by Gegelati's addLogger() API) and
 * ITrainingLogger (our typed callback interface). LALogger provides timing
 * helpers, the doValidation flag, and automatic registration with the agent.
 * ITrainingLogger exposes the arm-specific callbacks (logAfterTrainingValidate,
 * logEnvironmentStatus) that LALogger does not have.
 *
 * The only improvement over the original: result metrics are read through
 * IArmEvaluationResult instead of dynamic_pointer_cast to the concrete
 * ArmLearnEvaluationResult type, making the logger resilient to future
 * subclasses of the evaluation result.
 */
class ArmLearnLogger : public LALogger, public ITrainingLogger {
public:

    /**
     * @param la                    The LearningAgent to attach to (passed to LALogger).
     * @param doTrainingValidation  Whether a training-validation phase is active.
     * @param doUpdateLimits        Whether curriculum limit columns should be printed.
     * @param doControlTrajDeletion Whether the trajectory-deletion column is printed.
     * @param out                   Output stream (defaults to std::cout).
     */
    explicit ArmLearnLogger(Learn::LearningAgent& la,
                            bool doTrainingValidation  = false,
                            bool doUpdateLimits        = false,
                            bool doControlTrajDeletion = true,
                            std::ostream& out          = std::cout)
        : LALogger(la, out),
          doTrainingValidation(doTrainingValidation),
          doUpdateLimits(doUpdateLimits),
          doControlTrajDeletion(doControlTrajDeletion)
    {
        *this << std::setprecision(2) << std::fixed << std::right;
        logHeader();
    }

    // -----------------------------------------------------------------------
    // LALogger pure-virtual overrides
    // -----------------------------------------------------------------------

    void logHeader() override {
        // First header row
        *this << std::setw(2 * colW) << ' ' << std::setw(colW) << "Train";
        if (doValidation)         *this << std::setw(2 * colW) << ' ' << std::setw(colW) << "Valid";
        if (doTrainingValidation) *this << std::setw(2 * colW) << ' ' << std::setw(colW) << "Train Valid";
        *this << '\n';

        // Second header row — column names
        *this << std::setw(colW) << "Gen"
              << std::setw(colW) << "NbVert"
              << std::setw(colW) << "tRewAvg"
              << std::setw(colW) << "tRewMax"
              << std::setw(colW) << "tDistMax";

        if (doValidation)
            *this << std::setw(colW) << "vRewAvg"
                  << std::setw(colW) << "vRewMax"
                  << std::setw(colW) << "vDistMax"
                  << std::setw(colW) << "vSuccess";

        if (doTrainingValidation)
            *this << std::setw(colW) << "tvRewAvg"
                  << std::setw(colW) << "tvRewMax"
                  << std::setw(colW) << "tvDistMax";

        if (doUpdateLimits)
            *this << std::setw(colW) << "S_Targ"
                  << std::setw(colW) << "S_StartP";

        if (doControlTrajDeletion)
            *this << std::setw(colW) << "T_Del";

        *this << std::setw(colW) << "T_mutat"
              << std::setw(colW) << "T_eval";
        if (doValidation)         *this << std::setw(colW) << "T_val";
        if (doTrainingValidation) *this << std::setw(colW) << "T_TrVal";
        *this << std::setw(colW) << "T_total" << '\n';
    }

    void logNewGeneration(uint64_t& generationNumber) override {
        *this << std::setw(colW) << generationNumber;
        chronoFromNow();
    }

    void logAfterPopulateTPG() override {
        mutationTime = getDurationFrom(*checkpoint);
        *this << std::setw(colW) << learningAgent.getTPGGraph()->getNbVertices();
        chronoFromNow();
    }

    void logAfterEvaluate(
        std::multimap<std::shared_ptr<Learn::EvaluationResult>,
                      const TPG::TPGVertex*>& results) override
    {
        evalTime = getDurationFrom(*checkpoint);
        logResultColumns(results);
        chronoFromNow();
    }

    void logAfterDecimate() override {}   // nothing to log

    void logAfterValidate(
        std::multimap<std::shared_ptr<Learn::EvaluationResult>,
                      const TPG::TPGVertex*>& results) override
    {
        validTime = getDurationFrom(*checkpoint);
        logResultColumns(results);

        // Success rate — only meaningful in validation mode
        const auto& top = std::prev(results.end())->first;
        if (const auto* arm = dynamic_cast<const Learn::IArmEvaluationResult*>(top.get()))
            *this << std::setw(colW) << arm->getSuccess();

        chronoFromNow();
    }

    void logEndOfTraining() override {
        *this << std::setw(colW) << mutationTime
              << std::setw(colW) << evalTime;
        if (doValidation)         *this << std::setw(colW) << validTime;
        if (doTrainingValidation) *this << std::setw(colW) << trainingValidTime;
        *this << std::setw(colW) << getDurationFrom(*start) << '\n';
    }

    // -----------------------------------------------------------------------
    // ITrainingLogger extras (not in LALogger)
    // -----------------------------------------------------------------------

    void logAfterTrainingValidate(
        std::multimap<std::shared_ptr<Learn::EvaluationResult>,
                      const TPG::TPGVertex*>& results) override
    {
        trainingValidTime = getDurationFrom(*checkpoint);
        logResultColumns(results);
    }

    void logEnvironmentStatus(double targetRange, double startPos) override {
        *this << std::setw(colW) << targetRange
              << std::setw(colW) << startPos;
    }

private:
    const int colW = 10;

    bool doTrainingValidation;
    bool doUpdateLimits;
    bool doControlTrajDeletion;

    double trainingValidTime = 0.0;

    void logResultColumns(
        const std::multimap<std::shared_ptr<Learn::EvaluationResult>,
                            const TPG::TPGVertex*>& results)
    {
        if (results.empty()) return;

        double avgScore = 0.0;
        for (const auto& [res, _] : results) avgScore += res->getResult();
        avgScore /= static_cast<double>(results.size());

        const auto& top = std::prev(results.end())->first;
        double maxDist  = 0.0;
        if (const auto* arm = dynamic_cast<const Learn::IArmEvaluationResult*>(top.get()))
            maxDist = arm->getDistance();

        *this << std::setw(colW) << avgScore
              << std::setw(colW) << top->getResult()
              << std::setw(colW) << maxDist;
    }
};

} // namespace Log

#endif // ARM_LEARN_LOGGER_H