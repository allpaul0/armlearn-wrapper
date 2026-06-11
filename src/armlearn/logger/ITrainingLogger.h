#ifndef I_TRAINING_LOGGER_H
#define I_TRAINING_LOGGER_H

#include <map>
#include <memory>
#include <learn/evaluationResult.h>
#include <gegelati.h>

namespace Log {

/**
 * @brief Extension interface for arm-specific logger callbacks that LALogger
 * does not have.
 *
 * Signatures of the shared callbacks (logNewGeneration, logAfterEvaluate,
 * logAfterValidate, logEndOfTraining) intentionally match LALogger exactly —
 * same parameter types, same constness — so a single override in
 * ArmLearnLogger satisfies both base classes without ambiguity.
 *
 * The two methods declared here (logAfterTrainingValidate,
 * logEnvironmentStatus) are genuinely new; they have no counterpart in
 * LALogger.
 */
class ITrainingLogger {
public:
    virtual ~ITrainingLogger() = default;

    // Match LALogger signatures exactly so ArmLearnLogger overrides satisfy both.
    virtual void logNewGeneration(uint64_t& gen) = 0;

    virtual void logAfterEvaluate(
        std::multimap<std::shared_ptr<Learn::EvaluationResult>,
                      const TPG::TPGVertex*>& results) = 0;

    virtual void logAfterValidate(
        std::multimap<std::shared_ptr<Learn::EvaluationResult>,
                      const TPG::TPGVertex*>& results) = 0;

    virtual void logEndOfTraining() = 0;

    // Arm-specific extras not present in LALogger.
    virtual void logAfterTrainingValidate(
        std::multimap<std::shared_ptr<Learn::EvaluationResult>,
                      const TPG::TPGVertex*>& results) = 0;

    virtual void logEnvironmentStatus(double targetRange, double startPos) {}
};

} // namespace Log

#endif // I_TRAINING_LOGGER_H