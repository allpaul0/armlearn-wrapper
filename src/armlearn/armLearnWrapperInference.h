#ifndef ARM_LEARN_WRAPPER_INFERENCE_H
#define ARM_LEARN_WRAPPER_INFERENCE_H

#include "armLearnWrapper.h"
#include "../codegen/externHeader.h"

/**
 * Inference-mode variant of ArmLearnWrapper.
 *
 * The only behavioral difference from the base class is that data sources are
 * exposed as typeInf arrays (the type expected by the code-generated policy)
 * rather than as raw double arrays.  Everything else — trajectory management,
 * reward computation, collision detection, etc. — is inherited unchanged.
 */
class ArmLearnWrapperInference : public ArmLearnWrapper {

    /// Mirror of motorPos in the inference type
    Data::PrimitiveTypeArray<typeInf> motorPos_typeInf;

    /// Mirror of cartesianHand in the inference type
    Data::PrimitiveTypeArray<typeInf> cartesianHand_typeInf;

    /// Mirror of cartesianTarget in the inference type
    Data::PrimitiveTypeArray<typeInf> cartesianTarget_typeInf;

    /// Mirror of cartesianDiff in the inference type
    Data::PrimitiveTypeArray<typeInf> cartesianDiff_typeInf;

    /// Mirror of dataMotorSpeed in the inference type (only used when params.actionSpeed is true)
    Data::PrimitiveTypeArray<typeInf> dataMotorSpeed_typeInf;

protected:

    /**
     * @brief Override: fills both the base double arrays and the typeInf mirrors.
     */
    void computeInput() override;

public:

    /**
     * Constructor — same signature as ArmLearnWrapper.
     * Initialises the extra typeInf arrays to the same sizes as their double counterparts.
     */
    ArmLearnWrapperInference(int nbMaxActions, TrainingParameters& params,
                             bool algoIsDeterministic, bool handServosTrained = false)
        : ArmLearnWrapper(nbMaxActions, params, algoIsDeterministic, handServosTrained),
          motorPos_typeInf(6),
          cartesianHand_typeInf(3),
          cartesianTarget_typeInf(3),
          cartesianDiff_typeInf(3),
          dataMotorSpeed_typeInf(params.actionSpeed ? 4 : 0)
    {}

    /**
     * Copy constructor — mirrors the base copy constructor and copies the typeInf arrays.
     */
    ArmLearnWrapperInference(const ArmLearnWrapperInference& alwi)
        : ArmLearnWrapper(alwi),
          motorPos_typeInf(alwi.motorPos_typeInf),
          cartesianHand_typeInf(alwi.cartesianHand_typeInf),
          cartesianTarget_typeInf(alwi.cartesianTarget_typeInf),
          cartesianDiff_typeInf(alwi.cartesianDiff_typeInf),
          dataMotorSpeed_typeInf(alwi.dataMotorSpeed_typeInf)
    {}

    /**
     * @brief Override: returns typeInf arrays instead of double arrays,
     * so the code-generated policy receives data in its expected type.
     */
    std::vector<std::reference_wrapper<const Data::DataHandler>> getDataSources() override;

    /**
     * @brief Override: clones as ArmLearnWrapperInference rather than ArmLearnWrapper.
     */
    LearningEnvironment* clone() const override;

    /**
     * @brief Destructor — default is fine since all members are properly destructed by their own destructors.
     */
    virtual ~ArmLearnWrapperInference() = default;
};

#endif // ARM_LEARN_WRAPPER_INFERENCE_H