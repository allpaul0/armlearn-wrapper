#ifndef ARMLEARN_CONVERTER_ADAPTER_H
#define ARMLEARN_CONVERTER_ADAPTER_H

#include <vector>
#include <cstdint>

#include <armlearn/basiccartesianconverter.h>

#include "IKinematicsConverter.h"

/**
 * @brief Adapts armlearn::kinematics::Converter to IKinematicsConverter.
 *
 * We don't own the armlearn library so we cannot make its Converter class
 * implement our interface directly.  This adapter wraps the concrete armlearn
 * converter and forwards servoToCartesian() to
 * armlearn::kinematics::Converter::computeServoToCoord().
 *
 * ArmLearnWrapper constructs one of these around its existing `converter`
 * pointer and passes it to TrajectoryManager, keeping the external library
 * fully decoupled from our domain interfaces.
 */
class ArmlearnConverterAdapter : public IKinematicsConverter {
public:

    /**
     * @param converter  The armlearn converter to wrap.
     *                   Must outlive this adapter (owned by ArmLearnWrapper).
     */
    explicit ArmlearnConverterAdapter(armlearn::kinematics::Converter& converter)
        : converter(converter) {}

    std::vector<double> servoToCartesian(const std::vector<uint16_t>& motorPos) const override {
        return converter.computeServoToCoord(motorPos)->getCoord();
    }

private:
    armlearn::kinematics::Converter& converter;
};

#endif // ARMLEARN_CONVERTER_ADAPTER_H