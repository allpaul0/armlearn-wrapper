#include "ArmLearnWrapperInference.h"

// ---------------------------------------------------------------------------
// computeInput
// ---------------------------------------------------------------------------
// Delegates to the base class first so all double arrays are up to date, then
// copies each value into its typeInf mirror.  The order and sizes here must
// stay in sync with getDataSources().
// ---------------------------------------------------------------------------
void ArmLearnWrapperInference::computeInput() {

    ArmLearnWrapper::computeInput();

    auto mirror = [](const Data::PrimitiveTypeArray<double>& src,
                     Data::PrimitiveTypeArray<typeInf>&       dst,
                     int size) {
        for (int i = 0; i < size; ++i) {
            double val = *src.getDataAt(typeid(double), i)
                              .getSharedPointer<const double>();
            dst.setDataAt(typeid(typeInf), i, convEnvToInf(val));
        }
    };

    mirror(motorPos,        motorPos_inf,        6);
    mirror(cartesianHand,   cartesianHand_inf,   3);
    mirror(cartesianTarget, cartesianTarget_inf, 3);
    mirror(cartesianDiff,   cartesianDiff_inf,   3);

    if (params.actionSpeed)
        mirror(dataMotorSpeed, dataMotorSpeed_inf, 4);
}

// ---------------------------------------------------------------------------
// getDataSources
// ---------------------------------------------------------------------------
std::vector<std::reference_wrapper<const Data::DataHandler>>
ArmLearnWrapperInference::getDataSources() {
    auto result = std::vector<std::reference_wrapper<const Data::DataHandler>>();
    result.emplace_back(cartesianTarget_inf);
    result.emplace_back(cartesianHand_inf);
    result.emplace_back(cartesianDiff_inf);
    result.emplace_back(motorPos_inf);
    if (params.actionSpeed) result.emplace_back(dataMotorSpeed_inf);
    return result;
}
