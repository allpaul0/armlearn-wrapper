/**
* \brief Executable for translating a .dot into a c file.
*/
#include <filesystem>

#include <gegelati.h>
#include "instructions.h"
#include "params/trainingParameters.h"
#include "params/codegenParameters.h"
#include "armLearnLogger.h"
#include "armLearnWrapper.h"
#include "armLearningAgent.h"

// Perform validation to identify used edges & vertices
void validation(float *score, int *nbActions, 
    const TPG::TPGVertex* root, ArmLearnWrapper& armLearnEnv, 
    const Learn::LearningParameters& params, Environment& env,
    const std::string& trace){
    
    std::ofstream ofs ("outLogs/" + trace + ".txt", std::ofstream::out);
    TPG::TPGExecutionEngineInstrumented tee(env);
    
    *nbActions = 0;
    int nbActionsEp = 0;
    int nbEpisodes = 0;
    *score = 0;

    armLearnEnv.reset(0, Learn::LearningMode::VALIDATION, nbEpisodes, 0);


    while(nbEpisodes < params.nbIterationsPerPolicyEvaluation){
        if (armLearnEnv.isTerminal() || nbActionsEp == params.maxNbActionsPerEval || *nbActions == 0){
            *score += armLearnEnv.getScore();
            armLearnEnv.reset(0, Learn::LearningMode::VALIDATION, nbEpisodes, 0);
            nbEpisodes++;
            nbActionsEp = 0;
        }
    	uint64_t actionID = ((TPG::TPGAction*)(tee.executeFromRoot(*root).first.back()))->getActionID();
        armLearnEnv.doAction((double) actionID);
        ofs << *score << std::endl;
        ofs << *nbActions << " " << actionID << std::endl;
        (*nbActions)++;
        nbActionsEp++;
    }
    *score /= params.nbIterationsPerPolicyEvaluation;
    std::cout << "Total score: " << *score << " in "  << *nbActions << " actions." << std::endl;
    ofs.close();
}

int main(int argc, char** argv ){

    // Check if outLogs/codegen exists, if not, create it
    std::string codeGenPath = "outLogs/codegen/";
    if(!std::filesystem::exists(codeGenPath)) std::filesystem::create_directories(codeGenPath);

    // Set the parameters of the armLearnWrapper from trainParams.json
    TrainingParameters trainingParams;
    trainingParams.loadParametersFromJson("params/trainParams.json");

    // Set the parameters for the learning process from params.json
    Learn::LearningParameters params;
    File::ParametersParser::loadParametersFromJson("params/params.json", params);

    CodeGenParameters codeGenParams;
    codeGenParams.loadParametersFromJson("params/codegenParams.json");

    // Create the instruction set for programs
	Instructions::Set set;
	fillInstructionSet(set, trainingParams);

    // Instantiate the LearningEnvironment
    ArmLearnWrapper armLearnEnv(params.maxNbActionsPerEval, trainingParams, true);

    // Instantiate and init the learning agent
    Learn::ArmLearningAgent la(armLearnEnv, set, params, trainingParams);
    la.init(trainingParams.seed);

    // Load graph
    std::cout << "Loading dot file from " << trainingParams.tpgDotPathTraining << std::endl;
    auto &tpg = *la.getTPGGraph();
    Environment env = tpg.getEnvironment();
    TPG::TPGGraph tpgGraph(env, std::make_unique<TPG::TPGFactoryInstrumented>());
    File::TPGGraphDotImporter dot((trainingParams.tpgDotPathTraining).c_str(), env, tpgGraph);
    dot.importGraph();
    const TPG::TPGVertex* root = tpgGraph.getRootVertices().front();
    armLearnEnv.loadValidationTrajectories();

    /**** Play the game once to identify useful edges & vertices ****/
    std::cout << "Play with TPG from the GEGELATI lib" << std::endl;
    float scoreGegelati; int nbActionsGegelati;
    validation(&scoreGegelati, &nbActionsGegelati, root, armLearnEnv, params, env, "tpg_gegelati_validation");

    /**** Prune the unused vertices & teams ****/
    ((const TPG::TPGFactoryInstrumented&)tpgGraph.getFactory()).clearUnusedTPGGraphElements(tpgGraph);
    tpgGraph.clearProgramIntrons();

    root = tpgGraph.getRootVertices().front();

    /**** Play the game again to check the result remains the same ****/
    std::cout << "Play with code generated TPG" << std::endl;
    float scoreCodeGen; int nbActionsCodeGen;
    validation(&scoreCodeGen, &nbActionsCodeGen, root, armLearnEnv, params, env, "tpg_codegen_validation");

    if(scoreCodeGen != scoreGegelati || nbActionsCodeGen != nbActionsGegelati){
        std::cout << "Determinism was lost during graph pruning." << std::endl;
        exit(1);
    }

    // Get stats on graph to get the required stack size
    std::cout << "Analyze graph." << std::endl;
    TPG::PolicyStats ps;
    ps.setEnvironment(env);
    ps.analyzePolicy(tpgGraph.getRootVertices().front());
    std::ofstream bestStats;
    std::string bestPolicyStatsPath = codeGenPath + "best_root_pruned_stats.md";
    bestStats.open(bestPolicyStatsPath);
    bestStats << ps;
    bestStats.close();

    // Export pruned dot file
    std::cout << "Printing pruned dot file." << std::endl;
    std::string bestDot = codeGenPath + "best_root_pruned.dot";
    File::TPGGraphDotExporter dotExporter(bestDot.c_str(), tpgGraph);
    dotExporter.print();

    File::TPGGraphDotImporter dotImporter(bestDot.c_str(), env, tpg);
    // Compare the TPGGraph objects themselves (pointer equality)
    std::cout << "Comparing imported dot file to pruned TPGGraph." << std::endl;
    if (la.getTPGGraph().get() == &tpg)
        std::cout << "Dot import/export works correctly." << std::endl;
    else
        std::cout << "Dot import/export does not work correctly." << std::endl;

    trainingParams.testing = true;
    la.testingBestRoot(params.nbIterationsPerPolicyEvaluation);

    std::cout << "Printing C code." << std::endl;
	CodeGen::TPGGenerationEngineFactory factory(CodeGen::TPGGenerationEngineFactory::gotoMode);
    std::unique_ptr<CodeGen::TPGGenerationEngine> tpggen = factory.create("TPG", tpgGraph, codeGenPath, 
        trainingParams.instrType, codeGenParams.isInstrumented, codeGenParams.isDecorated);
    tpggen->generateTPGGraph();

    return 0;
}
