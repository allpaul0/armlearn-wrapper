/**
 * \brief Executable de generation de code (codegen) a partir d'un .dot
 *        d'entrainement.
 *
 *  ---------------------------------------------------------------------------
 *  ROLE ET CONTRAT AVEC exportLEstates
 *  ---------------------------------------------------------------------------
 *  Ce programme :
 *    1. importe le .dot d'entrainement (trainingParams.tpgDotPathTraining) ;
 *    2. joue des ROLLOUTS MULTI-ACTIONS pour marquer les elements reellement
 *       utilises (instrumentation cumulative) ;
 *    3. prune les elements inutilises + supprime les introns ;
 *    4. verifie que le determinisme est conserve apres pruning ;
 *    5. exporte le graphe PRUNE dans outLogs/codegen/best_root_pruned.dot ;
 *    6. genere le code C correspondant.
 *
 *  Le fichier best_root_pruned.dot est le CONTRAT avec exportLEstates :
 *  c'est exactement le graphe qui a servi a produire le code C. exportLEstates
 *  doit importer CE fichier (et non le .dot d'entrainement) pour que les
 *  identifiants de Teams, les parcours captures et LE_states.h soient coherents
 *  avec le code genere.
 *
 *  => Ordre d'execution obligatoire : codegen puis exportLEstates.
 */

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include <gegelati.h>

#include "instructions.h"
#include "params/trainingParameters.h"
#include "params/codegenParameters.h"
#include "armlearn/armLearnLogger.h"
#include "armlearn/armLearnWrapper.h"
#include "armlearn/armLearningAgent.h"

/// Rollout multi-actions de validation, utilise pour identifier les elements
/// utiles (avant pruning) puis pour verifier le determinisme (apres pruning).
void validation(float* score, int* nbActions,
                const TPG::TPGVertex* root, ArmLearnWrapper& armLearnEnv,
                const Learn::LearningParameters& params, Environment& env,
                const std::string& trace);

// =============================================================================
// MAIN
// =============================================================================
int main(int argc, char** argv)
{
    std::cout << "\033[1;33m=====[ codegen ]=====\033[0m" << std::endl;

    // Cree outLogs/codegen si absent
    std::string codeGenPath = "outLogs/codegen/";
    if (!std::filesystem::exists(codeGenPath))
        std::filesystem::create_directories(codeGenPath);

    // -------------------------------------------------------------------------
    // Chargement des parametres
    // -------------------------------------------------------------------------
    TrainingParameters trainingParams;
    trainingParams.loadParametersFromJson("params/trainParams.json");

    Learn::LearningParameters params;
    File::ParametersParser::loadParametersFromJson("params/params.json", params);

    CodeGenParameters codeGenParams;
    codeGenParams.loadParametersFromJson("params/codegenParams.json");

    // Jeu d'instructions
    Instructions::Set set;
    fillInstructionSet(set, trainingParams);

    // Learning Environment (simulateur de bras robotise)
    ArmLearnWrapper armLearnEnv(params.maxNbActionsPerEval, trainingParams, true);

    // Learning Agent
    Learn::ArmLearningAgent la(armLearnEnv, set, params, trainingParams);
    la.init(trainingParams.seed);

    // -------------------------------------------------------------------------
    // Chargement du graphe d'entrainement dans un TPGGraph instrumente
    // -------------------------------------------------------------------------
    std::cout << "Loading dot file from " << trainingParams.tpgDotPathTraining << std::endl;
    auto& tpg = *la.getTPGGraph();
    Environment env = tpg.getEnvironment();
    TPG::TPGGraph tpgGraph(env, std::make_unique<TPG::TPGFactoryInstrumented>());
    File::TPGGraphDotImporter dot((trainingParams.tpgDotPathTraining).c_str(), env, tpgGraph);
    dot.importGraph();
    const TPG::TPGVertex* root = tpgGraph.getRootVertices().front();
    armLearnEnv.loadValidationTrajectories();

    // -------------------------------------------------------------------------
    // PHASE 1 - Identification des elements utiles (rollout multi-actions).
    //
    // Le rollout multi-actions est indispensable ici : c'est lui qui fait
    // atteindre au LE des etats inaccessibles depuis reset(seed), et donc qui
    // fait visiter des Teams inaccessibles depuis l'etat initial. Un rollout
    // mono-action prunerait a tort ces elements.
    // -------------------------------------------------------------------------
    std::cout << "Play with TPG from the GEGELATI lib" << std::endl;
    float scoreGegelati; int nbActionsGegelati;
    validation(&scoreGegelati, &nbActionsGegelati, root, armLearnEnv, params, env,
               "tpg_gegelati_validation");

    // -------------------------------------------------------------------------
    // Pruning des elements non utilises + suppression des introns
    // -------------------------------------------------------------------------
    ((const TPG::TPGFactoryInstrumented&)tpgGraph.getFactory()).clearUnusedTPGGraphElementsV2(tpgGraph);
    tpgGraph.clearProgramIntrons();
    root = tpgGraph.getRootVertices().front();
    armLearnEnv.resetIterations();

    // -------------------------------------------------------------------------
    // PHASE 2 - Verification du determinisme apres pruning (memes rollouts)
    // -------------------------------------------------------------------------
    std::cout << "Play with code generated TPG" << std::endl;
    float scoreCodeGen; int nbActionsCodeGen;
    validation(&scoreCodeGen, &nbActionsCodeGen, root, armLearnEnv, params, env,
               "tpg_codegen_validation");

    if (scoreCodeGen != scoreGegelati || nbActionsCodeGen != nbActionsGegelati) {
        std::cout << "Determinism was lost during graph pruning." << std::endl;
        exit(1);
    }

    // -------------------------------------------------------------------------
    // Stats sur le graphe (necessaire pour la taille de pile requise)
    // -------------------------------------------------------------------------
    std::cout << "Analyze graph." << std::endl;
    TPG::PolicyStats ps;
    ps.setEnvironment(env);
    ps.analyzePolicy(tpgGraph.getRootVertices().front());
    std::ofstream bestStats;
    std::string bestPolicyStatsPath = codeGenPath + "best_root_pruned_stats.md";
    bestStats.open(bestPolicyStatsPath);
    bestStats << ps;
    bestStats.close();

    // -------------------------------------------------------------------------
    // Export du .dot PRUNE.
    // /!\ C'est l'entree de exportLEstates : ne pas renommer sans mettre a jour
    //     le chemin cote exportLEstates.
    // -------------------------------------------------------------------------
    std::cout << "Printing pruned dot file." << std::endl;
    std::string bestDot = codeGenPath + "best_root_pruned.dot";
    File::TPGGraphDotExporter dotExporter(bestDot.c_str(), tpgGraph);
    dotExporter.print();

    // Compare le TPGGraph re-importe au TPGGraph courant (egalite de pointeur)
    File::TPGGraphDotImporter dotImporter(bestDot.c_str(), env, tpg);
    std::cout << "Comparing imported dot file to pruned TPGGraph." << std::endl;
    (la.getTPGGraph().get() == &tpg)
        ? std::cout << "Dot import/export works correctly." << std::endl
        : std::cout << "Dot import/export does not work correctly." << std::endl;

    trainingParams.testing = true;
    la.testingBestRoot(params.nbIterationsPerPolicyEvaluation);

    // -------------------------------------------------------------------------
    // Generation du code C
    // -------------------------------------------------------------------------
    std::cout << "Printing C code." << std::endl;
    CodeGen::TPGGenerationEngineFactory factory(CodeGen::TPGGenerationEngineFactory::gotoMode);
    std::unique_ptr<CodeGen::TPGGenerationEngine> tpggen =
        factory.create("TPG", tpgGraph, codeGenPath, trainingParams.instrType, 
                        codeGenParams.teamInstrumented, codeGenParams.teamDecorated,
                        codeGenParams.dispatchInstrumented, codeGenParams.dispatchDecorated);
    tpggen->generateTPGGraph();

    std::cout << "\nPruned graph exported to " << bestDot << std::endl;
    std::cout << "End program" << std::endl;
    return 0;
}

// =============================================================================
// Rollout multi-actions de validation
// =============================================================================
void validation(float* score, int* nbActions,
                const TPG::TPGVertex* root, ArmLearnWrapper& armLearnEnv,
                const Learn::LearningParameters& params, Environment& env,
                const std::string& trace)
{
    std::ofstream ofs("outLogs/" + trace + ".txt", std::ofstream::out);
    TPG::TPGExecutionEngineInstrumented tee(env);
    armLearnEnv.reset(0, Learn::LearningMode::VALIDATION);

    int nbActionsEp = 0;
    int nbEpisodes = 0;
    *nbActions = 0;
    *score = 0;

    std::cout << params.nbIterationsPerPolicyEvaluation << " episodes of max "
              << params.maxNbActionsPerEval << " actions each." << std::endl;

    while (nbEpisodes < params.nbIterationsPerPolicyEvaluation) {
        if (armLearnEnv.isTerminal() || nbActionsEp == params.maxNbActionsPerEval || *nbActions == 0) {
            *score += armLearnEnv.getScore();
            armLearnEnv.reset(0, Learn::LearningMode::VALIDATION);
            nbEpisodes++;
            std::cout << "Episode " << nbEpisodes << " done. in " << nbActionsEp
                      << " actions. Current score: " << *score << std::endl;
            nbActionsEp = 0;
        }
        uint64_t actionID = ((TPG::TPGAction*)(tee.executeFromRoot(*root).first.back()))->getActionID();
        armLearnEnv.doAction((double)actionID);
        ofs << *score << std::endl;
        ofs << *nbActions << " " << actionID << std::endl;
        (*nbActions)++;
        nbActionsEp++;
    }
    *score /= params.nbIterationsPerPolicyEvaluation;
    std::cout << "Total score: " << *score << " in " << *nbActions << " actions." << std::endl;
    ofs.close();
}