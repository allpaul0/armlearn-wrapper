#include <iostream>
#include <filesystem>

#include "codegen/externHeader.h"
#include "codegen/TPG.h"
#include "armlearn/armLearnWrapperInference.h"
#include "instructions.h"
#include "params/trainingParameters.h"

/**
 * Accuracy benchmark for TPG inference, using validation trajectories 
 */

int main() {

    constexpr size_t g_numInferenceEpisodes = 100;
    constexpr size_t g_stepsPerEpisode = 1500;

    TrainingParameters trainingParams;
    trainingParams.loadParametersFromJson("params/trainParams.json");
    trainingParams.testing = true;

    // Instantiate the LearningEnvironment
    ArmLearnWrapperInference armLearnEnv(g_stepsPerEpisode, trainingParams, true);

    std::vector<typeInf*> inputs;

	/// fetch data in the environment
	auto dataSources = armLearnEnv.getDataSources();
    for (size_t i = 0; i < dataSources.size(); ++i)
    {
        inputs.push_back(
            dataSources.at(i)
                .get()
                .getDataAt(typeid(typeInf), 0)
                .getSharedPointer<typeInf>()
                .get());
    }
    armLearnEnv.loadValidationTrajectories();

    int nbEpisodes = 0;
    double scoreInf = 0;
    int nbActionsEp = 0;
    int nbActions = 0;
    std::cout << "Play with TPG code" << std::endl;
    while(nbEpisodes < g_numInferenceEpisodes){
        if (armLearnEnv.isTerminal() || nbActionsEp == g_stepsPerEpisode || nbActions == 0){
            scoreInf += (nbActions == 0) ? 0 : armLearnEnv.getScore();
            nbActionsEp = 0;
            armLearnEnv.reset(nbActions, Learn::LearningMode::VALIDATION);
            nbEpisodes++;
        }
    	typeInf actionID = -1;
        inferenceTPG(&actionID, inputs[0], inputs[1], inputs[2], inputs[3]);
        armLearnEnv.doAction((double) actionID);
        nbActionsEp++;
        nbActions++;
    }
    scoreInf /= 100;
    constexpr bool USING_GEGELATI = true; 
    armLearnEnv.logTestingTrajectories(USING_GEGELATI, "outLogs/codegen");
    std::cout << "Score at inference: " << scoreInf << std::endl;
}