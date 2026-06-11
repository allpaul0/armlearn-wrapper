#ifndef ARM_LEARN_UTILS_H
#define ARM_LEARN_UTILS_H

#include <vector>
#include <cstddef>
#include <algorithm>

/**
 * @brief Free utility functions shared across the armlearn-wrapper layer.
 *
 * Placed here so that TrajectoryManager and ArmLearnWrapper can both call
 * squaredError without either depending on the other, and without relying on
 * the inherited DeviceLearner::computeSquaredError() that is not available
 * outside the wrapper class hierarchy.
 */
namespace ArmLearnUtils {

/**
 * @brief Sum of squared differences between two Cartesian coordinate vectors.
 *
 * Used as a proxy for Euclidean distance everywhere in the codebase.
 * Both vectors are expected to have size 3 ([x, y, z]); extra elements are
 * silently ignored and missing elements are treated as zero difference.
 */
inline double squaredError(const std::vector<double>& a,
                           const std::vector<double>& b)
{
    double sum = 0.0;
    const std::size_t n = std::min(a.size(), b.size());
    for (std::size_t i = 0; i < n; ++i) {
        const double diff = a[i] - b[i];
        sum += diff * diff;
    }
    return sum;
}

} // namespace ArmLearnUtils

#endif // ARM_LEARN_UTILS_H