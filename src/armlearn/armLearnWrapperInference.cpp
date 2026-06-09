#include "armLearnWrapperInference.h"

// ---------------------------------------------------------------------------
// computeInput
// ---------------------------------------------------------------------------
// Calls the base implementation first (which updates all double arrays),
// then converts each value into its typeInf mirror via convEnvToInf().
// ---------------------------------------------------------------------------
void ArmLearnWrapperInference::computeInput() {

    // Let the base class fill all double arrays as normal.
    ArmLearnWrapper::computeInput();

    // --- motorPos ---
    for (int i = 0; i < 6; i++) {
        double val = *(motorPos.getDataAt(typeid(double), i).getSharedPointer<const double>());
        motorPos_typeInf.setDataAt(typeid(typeInf), i, convEnvToInf(val));
    }

    // --- cartesianHand, cartesianTarget, cartesianDiff (all size 3) ---
    for (int i = 0; i < 3; i++) {
        double hand = *(cartesianHand.getDataAt(typeid(double), i).getSharedPointer<const double>());
        cartesianHand_typeInf.setDataAt(typeid(typeInf), i, convEnvToInf(hand));

        double target = *(cartesianTarget.getDataAt(typeid(double), i).getSharedPointer<const double>());
        cartesianTarget_typeInf.setDataAt(typeid(typeInf), i, convEnvToInf(target));

        double diff = *(cartesianDiff.getDataAt(typeid(double), i).getSharedPointer<const double>());
        cartesianDiff_typeInf.setDataAt(typeid(typeInf), i, convEnvToInf(diff));
    }

    // --- dataMotorSpeed (only populated when params.actionSpeed is true) ---
    if (params.actionSpeed) {
        for (int i = 0; i < 4; i++) {
            double speed = *(dataMotorSpeed.getDataAt(typeid(double), i).getSharedPointer<const double>());
            dataMotorSpeed_typeInf.setDataAt(typeid(typeInf), i, convEnvToInf(speed));
        }
    }
}

// ---------------------------------------------------------------------------
// getDataSources
// ---------------------------------------------------------------------------
// Returns the typeInf mirrors instead of the double arrays so that the
// code-generated inference policy receives data in its expected type.
// ---------------------------------------------------------------------------
std::vector<std::reference_wrapper<const Data::DataHandler>>
ArmLearnWrapperInference::getDataSources() {

    auto result = std::vector<std::reference_wrapper<const Data::DataHandler>>();
    result.emplace_back(cartesianTarget_typeInf);
    result.emplace_back(cartesianHand_typeInf);
    result.emplace_back(cartesianDiff_typeInf);
    result.emplace_back(motorPos_typeInf);
    if (params.actionSpeed) result.emplace_back(dataMotorSpeed_typeInf);
    return result;
}

// ---------------------------------------------------------------------------
// clone
// ---------------------------------------------------------------------------
Learn::LearningEnvironment* ArmLearnWrapperInference::clone() const {
    return new ArmLearnWrapperInference(*this);
}