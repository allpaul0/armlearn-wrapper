#ifndef I_EVALUATION_RESULT_H
#define I_EVALUATION_RESULT_H

#include <learn/evaluationResult.h>

namespace Learn {

/**
 * @brief Extension contract for evaluation results that carry arm-specific
 * metrics (distance, success rate).
 *
 * Introducing this interface breaks the hard dependency that ArmLearnLogger
 * had on ArmLearnEvaluationResult via dynamic_pointer_cast. Any logger or
 * agent that only needs arm metrics can program against this interface.
 */
class IArmEvaluationResult {
public:
    virtual ~IArmEvaluationResult() = default;

    virtual double getSuccess()  const = 0;
    virtual double getDistance() const = 0;
};

} // namespace Learn

#endif // I_EVALUATION_RESULT_H
