#ifndef EVALUATION_POLICY_H
#define EVALUATION_POLICY_H

#include <vector>
#include <utility>
#include <cmath>
#include <numeric>

#include "../../params/trainingParameters.h"

namespace Learn {

/**
 * @brief Decides whether a root should be cancelled mid-evaluation and
 * post-processes the raw per-iteration scores into a final mean.
 *
 * Extracted from the inline logic in ArmLearningAgent::evaluateJob().
 */
class IEvaluationPolicy {
public:
    virtual ~IEvaluationPolicy() = default;

    /**
     * @brief Called after each iteration to check early-cancellation.
     *
     * @param scores           Per-iteration scores so far (including current).
     * @param generationNumber Current generation.
     * @param referenceBest    Average of the five last best scores.
     * @return true if the root should be cancelled.
     */
    virtual bool shouldCancel(const std::vector<double>& scores,
                              uint64_t generationNumber,
                              double   referenceBest) const = 0;

    /**
     * @brief Aggregate per-iteration scores into the final scalar used for
     * ranking (e.g. mean, mean - std).
     */
    virtual double aggregate(const std::vector<double>& scores) const = 0;
};


/**
 * @brief Default policy: cancels poor roots early, optionally penalises
 * high variance.
 */
class StandardEvaluationPolicy : public IEvaluationPolicy {
public:
    explicit StandardEvaluationPolicy(const TrainingParameters& params)
        : params(params) {}

    bool shouldCancel(const std::vector<double>& scores,
                      uint64_t generationNumber,
                      double   referenceBest) const override
    {
        if (generationNumber <= 5) return false;
        if (static_cast<int>(scores.size()) <= params.nbIterationRootCanceled) return false;

        const double mean = std::accumulate(scores.begin(), scores.end(), 0.0)
                          / static_cast<double>(scores.size());

        // Cancel if the running mean is more than twice the reference best
        return mean > 2.0 * referenceBest;
    }

    double aggregate(const std::vector<double>& scores) const override {
        if (scores.empty()) return 0.0;
        const double mean = std::accumulate(scores.begin(), scores.end(), 0.0)
                          / static_cast<double>(scores.size());

        if (!params.meanScoreWithStd) return mean;

        double sumVar = 0.0;
        for (double s : scores) sumVar += (s - mean) * (s - mean);
        const double stdDev = std::sqrt(sumVar / static_cast<double>(scores.size()));
        return mean - stdDev;
    }

private:
    const TrainingParameters& params;
};

} // namespace Learn

#endif // EVALUATION_POLICY_H
