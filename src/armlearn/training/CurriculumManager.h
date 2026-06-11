#ifndef CURRICULUM_MANAGER_H
#define CURRICULUM_MANAGER_H

#include "../../params/trainingParameters.h"
#include "../environment/TrajectoryManager.h"

/**
 * @brief Manages the progressive difficulty curriculum: decides when to widen
 * the target zone or the starting-position zone, then applies the update.
 *
 * Previously this logic lived inside ArmLearnWrapper::updateCurrentLimits(),
 * which tangled environment state with training schedule decisions. Extracting
 * it here lets the training loop (ArmLearningAgent) drive the curriculum
 * without touching the environment directly.
 */
class CurriculumManager {
public:

    CurriculumManager(TrainingParameters& params, TrajectoryManager& trajectoryManager)
        : params(params), trajectoryMgr(trajectoryManager) {}

    /**
     * @brief Call once per generation with the best distance achieved.
     *
     * @param bestDistance                 Best (lowest) distance from the
     *                                     training-validation evaluation.
     * @param nbIterationsPerPolicyEval    Used to rebuild the training-validation
     *                                     set when limits advance.
     * @return true if limits were updated this generation.
     */
    bool update(double bestDistance, int nbIterationsPerPolicyEval) {
        // Only advance when the best agent actually reaches the target
        if (bestDistance <= 0.0) {
            counterIterationUpgrade = 0;
            return false;
        }

        ++counterIterationUpgrade;
        if (counterIterationUpgrade < params.nbIterationsUpgrade) return false;

        // ---- advance limits ----
        if (params.progressiveModeTargets) {
            if (params.progressiveRangeTarget) {
                double r = trajectoryMgr.getCurrentRangeTarget();
                r = std::max(r * params.coefficientUpgradeMult,
                             r + params.coefficientUpgradeAdd);
                r = std::max(r, params.rangeTarget);
                trajectoryMgr.setCurrentRangeTarget(r);
            } else {
                double t = trajectoryMgr.getCurrentMaxLimitTarget();
                t = std::min(t * params.coefficientUpgradeMult,
                             t + params.coefficientUpgradeAdd);
                t = std::min(t, 750.0);
                trajectoryMgr.setCurrentMaxLimitTarget(t);
            }
        }

        if (params.progressiveModeStartingPos) {
            double sp = trajectoryMgr.getCurrentMaxLimitStartingPos();
            sp = std::min(sp * params.coefficientUpgradeMult,
                          sp + params.coefficientUpgradeAdd);
            sp = std::min(sp, 750.0);
            trajectoryMgr.setCurrentMaxLimitStartingPos(sp);
        }

        counterIterationUpgrade = 0;
        trajectoryMgr.updateTrainingValidation(nbIterationsPerPolicyEval);
        return true;
    }

    bool isActive() const {
        return params.progressiveModeTargets || params.progressiveModeStartingPos;
    }

private:
    TrainingParameters& params;
    TrajectoryManager&  trajectoryMgr;
    uint16_t counterIterationUpgrade = 0;
};

#endif // CURRICULUM_MANAGER_H
