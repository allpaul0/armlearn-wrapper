#ifndef I_KINEMATICS_CONVERTER_H
#define I_KINEMATICS_CONVERTER_H

#include <vector>
#include <cstdint>

/**
 * @brief Interface for converting motor positions to Cartesian coordinates.
 *
 * Extracted from ArmLearnWrapper's direct dependency on
 * armlearn::kinematics::BasicCartesianConverter. Any converter (real robot,
 * simulation, mock for tests) must implement this contract.
 */
class IKinematicsConverter {
public:
    virtual ~IKinematicsConverter() = default;

    /**
     * @brief Convert a vector of motor positions (uint16_t servo values) to
     * Cartesian coordinates [x, y, z].
     */
    virtual std::vector<double> servoToCartesian(const std::vector<uint16_t>& motorPos) const = 0;
};

#endif // I_KINEMATICS_CONVERTER_H
