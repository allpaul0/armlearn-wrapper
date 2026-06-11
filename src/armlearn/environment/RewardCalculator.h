#ifndef REWARD_CALCULATOR_H
#define REWARD_CALCULATOR_H

#include <cmath>
#include <vector>
#include "../../params/trainingParameters.h"

/**
 * @brief Encapsulates all reward/penalty logic previously scattered through
 * ArmLearnWrapper::computeReward() and ArmLearnWrapper::executeAction().
 *
 * The class is stateless with respect to episode progression — it receives
 * everything it needs as arguments so it is trivially unit-testable.
 */
class RewardCalculator {
public:
    explicit RewardCalculator(const TrainingParameters& params) : params(params) {}

    // -----------------------------------------------------------------------
    // Input bundle — everything the calculator needs for one step
    // -----------------------------------------------------------------------
    struct StepContext {
        double   distance;               ///< Current error (squared Euclidean)
        double   rangeTarget;            ///< Threshold to consider "reached"
        bool     givePenaltyUnavailable; ///< True if the action was out of bounds
        int      nbMotorMoving;          ///< Number of motors that actually moved
        int      nbActionsDone;          ///< Actions executed so far this episode
        int      nbMaxActions;           ///< Episode length cap
        bool     isMoving;               ///< False when arm is stuck / stopped
        bool     isCycling;              ///< True when a position cycle is detected
        bool     algoIsDeterministic;    ///< True for TPG, false for SAC
        const std::vector<double>& motorSpeed; ///< Speed per joint (used when actionSpeed)
    };

    // -----------------------------------------------------------------------
    // Result bundle
    // -----------------------------------------------------------------------
    struct StepResult {
        double reward;
        bool   terminalNow; ///< Whether this step triggers episode termination
    };

    // -----------------------------------------------------------------------
    // Main entry point
    // -----------------------------------------------------------------------
    StepResult compute(const StepContext& ctx) const {
        StepResult res{0.0, false};

        const double range = ctx.rangeTarget;

        // -- Threshold counter management is left to the caller (ArmLearnWrapper)
        // -- because it requires state across steps. The calculator only reads
        // -- the final terminal conditions that are passed in.

        // Terminal conditions
        if (!ctx.isMoving || ctx.isCycling) {
            res.terminalNow = true;
        }

        if (params.reachingObjectives && ctx.distance < range) {
            res.terminalNow = true;
            res.reward = 10.0;
            return res;
        }

        // penaltyStopTooSoon — multiplier when the arm stops prematurely
        const double penaltyStopTooSoon =
            ((!ctx.isMoving || ctx.isCycling) && ctx.algoIsDeterministic)
            ? static_cast<double>(ctx.nbMaxActions - ctx.nbActionsDone)
            : 1.0;

        // penaltyMoveUnavailable — only for non-deterministic algorithms
        const double penaltyMoveUnavailable =
            (ctx.givePenaltyUnavailable && !ctx.algoIsDeterministic)
            ? params.penaltyMoveUnavailable
            : 0.0;

        // penaltySpeed — joint speed or multi-motor penalty
        double penaltySpeed = 0.0;
        if (params.actionSpeed) {
            for (double s : ctx.motorSpeed)
                penaltySpeed += std::abs(s);
            penaltySpeed *= params.penaltySpeed;
        } else if (ctx.nbMotorMoving > 1) {
            penaltySpeed = static_cast<double>(ctx.nbMotorMoving - 1) * params.penaltySpeed;
        }

        res.reward = (-ctx.distance * params.coefRewardMultiplication
                      - penaltyMoveUnavailable
                      - penaltySpeed) * penaltyStopTooSoon;

        return res;
    }

    /**
     * @brief Compute the deterministic score override used by TPG after each
     * action. Replaces the inline logic in ArmLearnWrapper::executeAction().
     */
    double deterministicScore(double distance, int nbActionsDone, int nbMaxActions,
                               double currentRangeTarget, bool bonusNbIteration,
                               bool isValidation) const
    {
        double s = -distance;
        if (bonusNbIteration) {
            const double range = isValidation ? params.rangeTarget : currentRangeTarget;
            if (distance < range) {
                s = static_cast<double>(nbMaxActions - nbActionsDone) * params.coefRewardMultiplication;
            }
        }
        return s;
    }

private:
    const TrainingParameters& params;
};

#endif // REWARD_CALCULATOR_H
