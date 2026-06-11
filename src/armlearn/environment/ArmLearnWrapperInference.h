#ifndef ARM_LEARN_WRAPPER_INFERENCE_H
#define ARM_LEARN_WRAPPER_INFERENCE_H

#include "ArmLearnWrapper.h"
#include "../codegen/externHeader.h"

/**
 * @brief Inference-mode variant of ArmLearnWrapper.
 *
 * The only behavioural difference from the base class is that getDataSources()
 * returns typeInf arrays (the type expected by the code-generated policy)
 * instead of double arrays. Everything else — episode logic, reward, collision,
 * trajectory management — is inherited unchanged.
 *
 * Lifecycle:
 *   computeInput() is called by the base class after every action and on reset.
 *   This override lets it run first, then mirrors each double array into its
 *   typeInf counterpart via convEnvToInf().
 */
class ArmLearnWrapperInference : public ArmLearnWrapper {

    // typeInf mirrors — one per data source exposed by getDataSources()
    Data::PrimitiveTypeArray<typeInf> motorPos_inf;
    Data::PrimitiveTypeArray<typeInf> cartesianHand_inf;
    Data::PrimitiveTypeArray<typeInf> cartesianTarget_inf;
    Data::PrimitiveTypeArray<typeInf> cartesianDiff_inf;
    Data::PrimitiveTypeArray<typeInf> dataMotorSpeed_inf; ///< Zero-sized when !params.actionSpeed

protected:

    /**
     * @brief Calls the base implementation, then mirrors every double value
     * into the corresponding typeInf array.
     */
    void computeInput() override;

public:

    ArmLearnWrapperInference(int nbMaxActions,
                             TrainingParameters& params,
                             bool algoIsDeterministic,
                             bool handServosTrained = false)
        : ArmLearnWrapper(nbMaxActions, params, algoIsDeterministic, handServosTrained),
          motorPos_inf(6),
          cartesianHand_inf(3),
          cartesianTarget_inf(3),
          cartesianDiff_inf(3),
          dataMotorSpeed_inf(params.actionSpeed ? 4 : 0)
    {}

    ArmLearnWrapperInference(const ArmLearnWrapperInference& other)
        : ArmLearnWrapper(other),
          motorPos_inf(other.motorPos_inf),
          cartesianHand_inf(other.cartesianHand_inf),
          cartesianTarget_inf(other.cartesianTarget_inf),
          cartesianDiff_inf(other.cartesianDiff_inf),
          dataMotorSpeed_inf(other.dataMotorSpeed_inf)
    {}

    /**
     * @brief Returns typeInf arrays so the code-generated policy receives
     * data in its expected numeric type.
     */
    std::vector<std::reference_wrapper<const Data::DataHandler>>
    getDataSources() override;

    Learn::LearningEnvironment* clone() const override {
        return new ArmLearnWrapperInference(*this);
    }

    virtual ~ArmLearnWrapperInference() = default;
};

#endif // ARM_LEARN_WRAPPER_INFERENCE_H
