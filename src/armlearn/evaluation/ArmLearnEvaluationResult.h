#ifndef ARM_LEARN_EVALUATION_RESULT_H
#define ARM_LEARN_EVALUATION_RESULT_H

#include <memory>
#include <stdexcept>
#include <vector>
#include <cmath>

#include <learn/evaluationResult.h>
#include "IArmEvaluationResult.h"

namespace Learn {

/**
 * @brief Evaluation result carrying score, distance, success rate, and the
 * per-trajectory score list.
 *
 * Changes from original:
 *  - Implements the new IArmEvaluationResult interface so loggers and agents
 *    can program against an abstraction instead of a concrete type.
 *  - getTrajScores() returns a const reference to avoid a full copy on every call.
 *  - operator+= is guarded by the interface type, not by typeid.
 */
class ArmLearnEvaluationResult : public EvaluationResult, public IArmEvaluationResult {
public:

    ArmLearnEvaluationResult(double score, double success, double distance,
                              std::vector<std::pair<int, double>> trajScores,
                              size_t nbEval)
        : EvaluationResult(score, nbEval),
          success(success), distance(distance),
          trajScores(std::move(trajScores))
    {}

    // IArmEvaluationResult
    double getSuccess()  const override { return success; }
    double getDistance() const override { return distance; }

    const std::vector<std::pair<int, double>>& getTrajScores() const { return trajScores; }

    // -----------------------------------------------------------------------
    // Weighted combination (called by LearningAgent when reusing past evals)
    // -----------------------------------------------------------------------
    EvaluationResult& operator+=(const EvaluationResult& other) override {
        const auto* otherArm = dynamic_cast<const ArmLearnEvaluationResult*>(&other);
        if (!otherArm) {
            throw std::runtime_error("[ArmLearnEvaluationResult] Type mismatch in operator+=.");
        }

        const double totalN = static_cast<double>(this->nbEvaluation)
                            + static_cast<double>(otherArm->nbEvaluation);

        auto blend = [&](double a, double b) {
            return (a * this->nbEvaluation + b * otherArm->nbEvaluation) / totalN;
        };

        this->result   = blend(this->result,   otherArm->result);
        this->success  = blend(this->success,  otherArm->success);
        this->distance = blend(this->distance, otherArm->distance);
        this->nbEvaluation += otherArm->nbEvaluation;

        return *this;
    }

private:
    double success  = 0.0;
    double distance = 0.0;
    std::vector<std::pair<int, double>> trajScores;
};

} // namespace Learn

#endif // ARM_LEARN_EVALUATION_RESULT_H
