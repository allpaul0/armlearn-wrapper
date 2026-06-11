#ifndef ARM_LEARN_WRAPPER_H
#define ARM_LEARN_WRAPPER_H

#include <random>
#include <vector>
#include <string>
#include <cstdint>
#include <cmath>

#include <gegelati.h>
#include <armlearn/nowaitarmsimulator.h>
#include <armlearn/serialcontroller.h>
#include <armlearn/trajectory.h>
#include <armlearn/widowxbuilder.h>
#include <armlearn/basiccartesianconverter.h>
#include <armlearn/devicelearner.h>

#include "../../params/trainingParameters.h"
#include "DataSourceInfo.h"
#include "../kinematics/IKinematicsConverter.h"
#include "../kinematics/ICollisionDetector.h"
#include "../kinematics/WidowXCollisionDetector.h"
#include "../kinematics/ArmlearnConverterAdapter.h"
#include "ArmLearnUtils.h"
#include "TrajectoryManager.h"
#include "RewardCalculator.h"
#include "EpisodeRecorder.h"

/**
 * @brief Learning environment for the WidowX robotic arm.
 *
 * Compared to the original, this class has a single responsibility: run one
 * episode (reset → doAction loop → score). All other concerns have been
 * extracted:
 *
 *  - EpisodeTrajectory generation / persistence  → TrajectoryManager
 *  - Reward / penalty computation         → RewardCalculator
 *  - Per-step recording and CSV export    → EpisodeRecorder
 *  - Collision geometry                   → WidowXCollisionDetector
 *
 * The class still inherits from DeviceLearner to keep the existing armlearn
 * device abstraction, and from LearningEnvironment for Gegelati integration.
 */
class ArmLearnWrapper : public Learn::LearningEnvironment,
                        public armlearn::learning::DeviceLearner {
protected:

    // -----------------------------------------------------------------------
    // Parameters (owned externally)
    // -----------------------------------------------------------------------
    TrainingParameters& params;

    // -----------------------------------------------------------------------
    // Collaborators (owned by this class)
    // -----------------------------------------------------------------------
    armlearn::kinematics::Converter* converter;   ///< Kinematics (armlearn, raw ownership)
    ArmlearnConverterAdapter         converterAdapter; ///< Adapts converter to IKinematicsConverter
    WidowXCollisionDetector          collisionDetector;
    TrajectoryManager                trajectoryMgr;
    RewardCalculator                 rewardCalc;
    EpisodeRecorder                  recorder;

    // -----------------------------------------------------------------------
    // Gegelati data sources
    // -----------------------------------------------------------------------
    Data::PrimitiveTypeArray<double> motorPos;        ///< 6 servo positions
    Data::PrimitiveTypeArray<double> cartesianHand;   ///< [x, y, z] hand
    Data::PrimitiveTypeArray<double> cartesianTarget; ///< [x, y, z] target
    Data::PrimitiveTypeArray<double> cartesianDiff;   ///< target - hand
    Data::PrimitiveTypeArray<double> dataMotorSpeed;  ///< joint speeds (optional)

    // -----------------------------------------------------------------------
    // Episode state
    // -----------------------------------------------------------------------
    Mutator::RNG rng;

    bool   terminal        = false;
    bool   isMoving        = true;
    bool   isCycling       = false;
    bool   isValidation    = false;
    bool   algoIsDeterministic;
    bool   handServosTrained;

    double score           = 0.0;
    double distance        = 0.0;
    double reward          = 0.0;
    double timeEnv         = 0.0;    ///< Accumulated device I/O time (excluded from timing)

    int    nbMaxActions;
    size_t nbActionsDone   = 0;
    int    nbActionsInThreshold = 0;
    int    valKillCollision = 0;

    std::vector<double>              motorSpeed = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    std::vector<std::vector<uint16_t>> memoryMotorPos; ///< Cycle detection buffer

    armlearn::Input<double>*  currentTarget     = nullptr;
    std::vector<uint16_t>*    currentStartingPos = nullptr;

    int iterationNb = 0;   ///< Index into the current trajectory set

    // Timing checkpoint for doAction (excluded from episode time)
    std::shared_ptr<std::chrono::time_point<
        std::chrono::system_clock, std::chrono::nanoseconds>> checkpointEnv;

    // -----------------------------------------------------------------------
    // Internals
    // -----------------------------------------------------------------------
    virtual void computeInput();

    void executeAction(std::vector<double> motorAction);

    void updateAndCheckCycles();

    static double computeSquaredError(const std::vector<double>& a,
                                      const std::vector<double>& b) {
        double sum = 0.0;
        for (size_t i = 0; i < std::min(a.size(), b.size()); ++i)
            sum += (a[i] - b[i]) * (a[i] - b[i]);
        return sum;
    }

    /// Build the armlearn device + converter (called once in constructor).
    armlearn::communication::AbstractController* iniController() {
        auto* conv  = new armlearn::kinematics::BasicCartesianConverter();
        auto* sim   = new armlearn::communication::NoWaitArmSimulator(
                          armlearn::communication::none);
        armlearn::WidowXBuilder builder;
        builder.buildConverter(*conv);
        builder.buildController(*sim);
        sim->connect();
        sim->changeSpeed(50);
        sim->updateInfos();
        converter = conv;
        return sim;
    }

public:

    // -----------------------------------------------------------------------
    // Construction
    // -----------------------------------------------------------------------
    ArmLearnWrapper(int nbMaxActions,
                    TrainingParameters& params,
                    bool algoIsDeterministic,
                    bool handServosTrained = false)
        : Learn::LearningEnvironment(handServosTrained ? 13 : 9),
          armlearn::learning::DeviceLearner(iniController()),
          params(params),
          algoIsDeterministic(algoIsDeterministic),
          handServosTrained(handServosTrained),
          nbMaxActions(nbMaxActions),
          // converter is set by iniController() inside DeviceLearner(...) above,
          // so *converter is valid by the time this member initialiser runs.
          converterAdapter(*converter),
          // ValidatorFn lambda captures `this`; device is already initialised.
          trajectoryMgr(params, converterAdapter, collisionDetector, rng,
                        [this](std::vector<uint16_t> pos) {
                            return device->toValidPosition(pos);
                        }),
          rewardCalc(params),
          // Data arrays
          motorPos(6), cartesianHand(3), cartesianTarget(3), cartesianDiff(3),
          dataMotorSpeed(params.actionSpeed ? 4 : 0)
    {
        rng.setSeed(params.seed);
    }

    /// Copy constructor — required by Gegelati's parallel evaluation.
    ArmLearnWrapper(const ArmLearnWrapper& other)
        : Learn::LearningEnvironment(other.nbActions),
          armlearn::learning::DeviceLearner(iniController()),
          params(other.params),
          algoIsDeterministic(other.algoIsDeterministic),
          handServosTrained(other.handServosTrained),
          nbMaxActions(other.nbMaxActions),
          converterAdapter(*converter),          ///< New converter from iniController()
          trajectoryMgr(other.trajectoryMgr),   ///< Deep-copies trajectory sets
          rewardCalc(other.rewardCalc),
          motorPos(other.motorPos),
          cartesianHand(other.cartesianHand),
          cartesianTarget(other.cartesianTarget),
          cartesianDiff(other.cartesianDiff),
          dataMotorSpeed(other.dataMotorSpeed)
    {
        iterationNb = 0;
    }

    virtual ~ArmLearnWrapper() {
        delete device;
        delete converter;
    }

    // -----------------------------------------------------------------------
    // LearningEnvironment interface
    // -----------------------------------------------------------------------
    void doAction(double actionID) override;
    void reset(size_t seed = 0,
               Learn::LearningMode mode = Learn::LearningMode::TRAINING) override;

    std::vector<std::reference_wrapper<const Data::DataHandler>>
    getDataSources() override;

    double getScore()    const override;
    bool   isTerminal()  const override;
    bool   isCopyable()  const override;

    virtual Learn::LearningEnvironment* clone() const;

    // -----------------------------------------------------------------------
    // Continuous-action variant (SAC)
    // -----------------------------------------------------------------------
    void doActionContinuous(std::vector<float> actions);

    // -----------------------------------------------------------------------
    // EpisodeTrajectory management (thin delegation to TrajectoryManager)
    // -----------------------------------------------------------------------
    void updateTrainingTrajectories(int n)           { trajectoryMgr.updateTraining(n); }
    void updateValidationTrajectories(int n)         { trajectoryMgr.updateValidation(n); }
    void updateTrainingValidationTrajectories(int n) { trajectoryMgr.updateTrainingValidation(n); }
    void saveValidationTrajectories()                { trajectoryMgr.saveValidationTrajectories(); }
    void loadValidationTrajectories()                { trajectoryMgr.loadValidationTrajectories(); }
    void addToScoreTrajectories(int idx, double sc)  { trajectoryMgr.addScore(idx, sc); }
    size_t getNbPossibleTargets() const              { return trajectoryMgr.getNbPossibleTargets(); }

    /// Direct access to the TrajectoryManager — needed by CurriculumManager in main.
    TrajectoryManager&       getTrajectoryManager()       { return trajectoryMgr; }
    const TrajectoryManager& getTrajectoryManager() const { return trajectoryMgr; }

    void resetIterations() { iterationNb = 0; }

    // -----------------------------------------------------------------------
    // Curriculum-related accessors (read by CurriculumManager via TrajectoryManager)
    // -----------------------------------------------------------------------
    double getCurrentMaxLimitTarget()      const { return trajectoryMgr.getCurrentMaxLimitTarget(); }
    double getCurrentMaxLimitStartingPos() const { return trajectoryMgr.getCurrentMaxLimitStartingPos(); }
    double getCurrentRangeTarget()         const { return trajectoryMgr.getCurrentRangeTarget(); }

    // -----------------------------------------------------------------------
    // Testing / export
    // -----------------------------------------------------------------------
    void logTestingTrajectories(bool usingGegelati, const std::string& exportDir) {
        recorder.exportCSV(usingGegelati, exportDir);
    }

    // -----------------------------------------------------------------------
    // Misc accessors
    // -----------------------------------------------------------------------
    double               getDistance()         const { return distance; }
    double               getReward()           const { return reward; }
    bool                 getIsMoving()         const { return isMoving && !isCycling; }
    std::vector<uint16_t> getInitStartingPos() const { return trajectoryMgr.getInitStartingPos(); }
    std::vector<uint16_t> getMotorsPos();

    void setAlgoIsDeterministic(bool v)               { algoIsDeterministic = v; }
    void setIsMoving(bool v)                          { isMoving = v; isCycling = false; }
    void setTerminal(bool v)                          { terminal = v; }
    void setInitStartingPos(std::vector<uint16_t> p)  { trajectoryMgr.setInitStartingPos(std::move(p)); }
    void incrValKillCollision()                       { ++valKillCollision; }

    std::string toString()       const override;
    std::string newGoalToString() const;

    // DeviceLearner stubs
    virtual void learn() override {}
    virtual void test()  override {}
    virtual armlearn::Output<std::vector<uint16_t>>*
    produce(const armlearn::Input<uint16_t>&) override {
        return new armlearn::Output<std::vector<uint16_t>>(
            std::vector<std::vector<uint16_t>>());
    }

    std::vector<DataSourceInfo> getDataSourcesInfo() const;
};

#endif // ARM_LEARN_WRAPPER_H