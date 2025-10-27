#include <iostream>
#include <filesystem>

#include "externHeader.h"
#include "codeGenArmlearn.h"
#include "../armLearnWrapper.h"
#include "../instructions.h"
#include "../trainingParameters.h"

int main() {

    TrainingParameters trainingParams;
    trainingParams.loadParametersFromJson("params/trainParams.json");
    trainingParams.testing = true;

    // Instantiate the fake LearningEnvironment
    

    int nbEpisodes = 0;
    int nbActions = 0;
    std::cout << "Play with TPG code" << std::endl;
    while(nbEpisodes < 100){
        if (nbActions == 0){
            // init new episode
        }
    	typeInf actionID = -1;
        inferenceTPG(&actionID);
    }
    std::cout << "End inference benchmark" << std::endl;
}