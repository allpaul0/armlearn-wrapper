#ifndef TRAJECTORY_MANAGER_H
#define TRAJECTORY_MANAGER_H

#include <vector>
#include <utility>
#include <string>
#include <functional>
#include <cstdint>

#include <gegelati.h>
#include <armlearn/nowaitarmsimulator.h>
#include <armlearn/serialcontroller.h>
#include <armlearn/trajectory.h>
#include <armlearn/widowxbuilder.h>
#include <armlearn/basiccartesianconverter.h>
#include <armlearn/devicelearner.h>

#include "../../params/trainingParameters.h"
#include "../kinematics/IKinematicsConverter.h"
#include "../kinematics/ICollisionDetector.h"
#include "ArmLearnUtils.h"

/**
 * @brief A starting motor-position vector paired with a heap-allocated
 * Cartesian target. Named EpisodeTrajectory to avoid collision with
 * armlearn::Trajectory which is defined in <armlearn/trajectory.h>.
 */
using EpisodeTrajectory = std::pair<std::vector<uint16_t>*, armlearn::Input<double>*>;

/**
 * @brief Owns and manages all three trajectory sets (training, training-
 * validation, validation) and the pool of Cartesian target candidates loaded
 * from CSV.
 *
 * The class handles generation, persistence, score-guided deletion, and the
 * curriculum limits that gate how far from the home position a trajectory is
 * allowed to reach.
 *
 * ### Device dependency
 * The armlearn device exposes `toValidPosition()` which clamps raw servo
 * values to the physical range of the real robot.  TrajectoryManager cannot
 * own a device, so the caller injects this operation as a `ValidatorFn`
 * in the constructor.  ArmLearnWrapper passes a lambda that forwards to
 * `device->toValidPosition()`.
 */
class TrajectoryManager {
public:

    /// Signature of the device clamping function.
    using ValidatorFn = std::function<std::vector<uint16_t>(std::vector<uint16_t>)>;

    // -----------------------------------------------------------------------
    // Construction / destruction
    // -----------------------------------------------------------------------

    TrajectoryManager(TrainingParameters&   params,
                      IKinematicsConverter& converter,
                      ICollisionDetector&   collisionDetector,
                      Mutator::RNG&         rng,
                      ValidatorFn           toValidPosition);

    /// Copy constructor — required because ArmLearnWrapper is copyable.
    /// Deep-copies trajectory sets and preserves the injected callbacks.
    TrajectoryManager(const TrajectoryManager& other);

    ~TrajectoryManager();

    // -----------------------------------------------------------------------
    // EpisodeTrajectory set accessors
    // -----------------------------------------------------------------------
    std::vector<EpisodeTrajectory>& getTraining()           { return trainingTrajectories; }
    std::vector<EpisodeTrajectory>& getValidation()         { return validationTrajectories; }
    std::vector<EpisodeTrajectory>& getTrainingValidation() { return trainingValidationTrajectories; }

    // -----------------------------------------------------------------------
    // Curriculum limit accessors
    // -----------------------------------------------------------------------
    double getCurrentMaxLimitTarget()      const { return currentMaxLimitTarget; }
    double getCurrentMaxLimitStartingPos() const { return currentMaxLimitStartingPos; }
    double getCurrentRangeTarget()         const { return currentRangeTarget; }
    size_t getNbPossibleTargets()          const { return dataTarget.size(); }

    void setCurrentMaxLimitTarget(double v)      { currentMaxLimitTarget = v; }
    void setCurrentMaxLimitStartingPos(double v) { currentMaxLimitStartingPos = v; }
    void setCurrentRangeTarget(double v)         { currentRangeTarget = v; }

    // -----------------------------------------------------------------------
    // Starting position
    // -----------------------------------------------------------------------
    std::vector<uint16_t> getInitStartingPos() const     { return initStartingPos; }
    void setInitStartingPos(std::vector<uint16_t> pos)   { initStartingPos = std::move(pos); }

    // -----------------------------------------------------------------------
    // EpisodeTrajectory generation
    // -----------------------------------------------------------------------

    /**
     * @brief Partially clears the training set according to
     * params.propTrajectoriesReused, then refills up to nbTrajectories.
     */
    void updateTraining(int nbTrajectories);

    /**
     * @brief Clears and fully regenerates the training-validation set.
     * Called by CurriculumManager when limits advance.
     */
    void updateTrainingValidation(int nbTrajectories);

    /**
     * @brief Clears and fully regenerates the validation set.
     */
    void updateValidation(int nbTrajectories);

    /**
     * @brief Inserts a hand-crafted trajectory at the front of the chosen
     * set, evicting the current first entry.
     *
     * @param newGoal       Heap-allocated Cartesian target (ownership transferred).
     * @param startingPos   Starting motor positions (copied internally).
     * @param useValidation true  → insert into validationTrajectories,
     *                      false → insert into trainingTrajectories.
     *
     * NOTE: The original code had a bug here — it operated on a local copy of
     * the vector so mutations were silently discarded. This version operates
     * on a reference to the actual member.
     */
    void customTrajectory(armlearn::Input<double>*      newGoal,
                          const std::vector<uint16_t>&  startingPos,
                          bool                          useValidation = false);

    // -----------------------------------------------------------------------
    // Score tracking (feeds back into clearPropTraining)
    // -----------------------------------------------------------------------
    void addScore(int index, double score);

    // -----------------------------------------------------------------------
    // Persistence
    // -----------------------------------------------------------------------
    void saveValidationTrajectories(
        const std::string& path = "params/ValidationTrajectories.txt") const;

    void loadValidationTrajectories(
        const std::string& path = "params/ValidationTrajectories.txt");

private:

    // ---- References / callbacks -------------------------------------------
    TrainingParameters&   params;
    IKinematicsConverter& converter;
    ICollisionDetector&   collisionDetector;
    Mutator::RNG&         rng;
    ValidatorFn           toValidPosition;   ///< Injected from device

    // ---- State ------------------------------------------------------------
    std::vector<uint16_t>              initStartingPos;
    std::vector<EpisodeTrajectory>     trainingTrajectories;
    std::vector<EpisodeTrajectory>     validationTrajectories;
    std::vector<EpisodeTrajectory>     trainingValidationTrajectories;
    std::vector<std::vector<double>>   dataTarget;
    std::vector<std::pair<int,double>> scoreTrajectories;

    double currentMaxLimitTarget      = 10000.0;
    double currentMaxLimitStartingPos = 10000.0;
    double currentRangeTarget         = 5.0;

    // ---- Internal helpers -------------------------------------------------
    void clearAll();
    void clearVector(std::vector<EpisodeTrajectory>& v, bool deleteStartingPos);
    void clearPropTraining();

    void loadTargetCSV(const std::string& path = "params/AllTarget.csv");

    std::vector<uint16_t>*    generateRandomStartingPos(bool validation);
    armlearn::Input<double>*  randomGoal(const std::vector<uint16_t>& startingPos,
                                         bool validation);
    std::vector<uint16_t>     randomMotorPos(const std::vector<double>& cartGoal,
                                             bool validation, bool isTarget);
};

#endif // TRAJECTORY_MANAGER_H