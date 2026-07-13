/**
 * \brief Executable fusionnant la generation de code (codegen) et l'export des
 *        etats du Learning Environment (LE) pour l'InferenceBenchmark embarque.
 *
 *  ---------------------------------------------------------------------------
 *  POURQUOI CETTE FUSION ?
 *  ---------------------------------------------------------------------------
 *  Les deux scripts d'origine parcouraient le graphe de deux facons DIFFERENTES :
 *
 *   - Le script de codegen joue le jeu par ROLLOUTS MULTI-ACTIONS (plusieurs
 *     doAction() par episode). L'etat du LE evolue au fil de l'episode, ce qui
 *     permet au TPG de manipuler le Learning Environment et de lui faire atteindre
 *     des positions qui ne sont pas accessibles à partir de la fonction 
 *     d'initialisation (armLearnEnv.reset(seed)) 
 *     Ces parcours atteignables seulement apres plusieurs interactions impliquent
 *     l'exécution de Teams qui ne sont pas atteignables depuis l'etat initial.
 *     C'est ce parcours complet qui permet d'identifier les elements reellement
 *     utilises -> pruning + suppression des introns.
 *
 *   - Le script ExportLEstates capturait les etats du LE en n'executant QU'UNE
 *     SEULE action par seed (reset(seed) puis un unique executeFromRoot, sans
 *     doAction). Le LE ne quittait donc jamais son etat de depart : seuls les
 *     parcours declenches par les etats INITIAUX etaient captures. Resultat :
 *     couverture de graphe incomplete (des Teams n'apparaissent jamais dans
 *     LE_states.h), ce qui fausse l'InferenceBenchmark.
 *
 *  Le correctif de fond : la capture des etats du LE utilise desormais le MEME
 *  rollout multi-actions que la validation/pruning. On capture (etat, parcours)
 *  a CHAQUE pas de chaque episode, sur le graphe DEJA prune (donc coherent avec
 *  le code genere). Les etats profonds sont ainsi captures et la couverture de
 *  graphe redevient complete.
 *
 *  Note technique sur l'instrumentation : le pruning a besoin de compteurs
 *  d'instrumentation CUMULATIFS (savoir si un element a ete visite AU MOINS une
 *  fois sur tout le rollout), alors que la capture a besoin de compteurs ISOLES
 *  par execution (pour lire le parcours exact du pas courant via analyzeExecution).
 *  Ces deux besoins etant incompatibles sur les memes compteurs, la capture est
 *  un rollout dedie POST-pruning, mais qui reproduit exactement la meme mecanique
 *  episodique multi-actions -> memes etats atteints, couverture complete.
 * 
 *  Une fois la capture terminee, deux choix sont possibles :
 *  - Si l'on souhaite réaliser de l'inférence pour mesurer équitablement un TPG, 
 *  on conserve tous les parcours decouverts (mapITI complet) pour l'export LE_states.h
 *  - Si l'on souhaite faire de la modélisation des Teams du TPG, 
 *  on peut ne conserver qu'un sous-ensemble minimal de parcours couvrant tous les Teams 
 *  decouverts (mapITI minimal) pour l'export LE_states.h (minimalTeamCoverOnly = true).
 *  
 *
 *  L'export de la codegen (pruning, verif de determinisme, stats, .dot, testing,
 *  generation C) reste inchange.
 */

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include <gegelati.h>

#include "instructions.h"
#include "params/trainingParameters.h"
#include "params/codegenParameters.h"
#include "codegen/externHeader.h"
#include "armlearn/armLearnLogger.h"
#include "armlearn/armLearnWrapper.h"
#include "armlearn/armLearningAgent.h"

// -----------------------------------------------------------------------------
// Parametres de recherche de seeds pour l'equilibrage des classes de parcours
// -----------------------------------------------------------------------------
#define DEFAULT_NB_SEEDS_TO_SEARCH 2E2 // nb de trajectoires par serie de recherche
#define MAX_NB_SEEDS_TO_SEARCH     2E3 // borne pour eviter une boucle infinie
#define NB_VALUES_PER_CLASS        10  // nb d'occurences voulues par parcours de graphe

/// Si vrai, l'ordre des donnees est randomise dans le header de sortie.
/// Permet de distribuer le calcul et d'etre moins dependant de la chauffe du CPU.
bool randomizeSeeds = true;

/// If true, keep only a minimal set of traversals covering every Team,
/// instead of benchmarking all traversal classes.
bool minimalTeamCoverOnly = true;

// -----------------------------------------------------------------------------
// Declarations
// -----------------------------------------------------------------------------

/// Rollout multi-actions de validation utilise pour identifier les elements
/// utiles (pruning) puis pour verifier le determinisme apres pruning.
/// (Comportement identique au script de codegen d'origine.)
void validation(float* score, int* nbActions,
                const TPG::TPGVertex* root, ArmLearnWrapper& armLearnEnv,
                const Learn::LearningParameters& params, Environment& env,
                const std::string& trace);

/// Capture les etats du LE le long de rollouts MULTI-ACTIONS sur le graphe prune.
/// C'est le coeur du correctif : meme mecanique episodique que la validation,
/// capture de (etat, parcours) a chaque pas, jusqu'a equilibrer les classes.
std::map<std::list<int>, std::vector<TPG::InferenceTraceInfos>>
captureLEStatesAlongRollouts(const TPG::TPGVertex* root,
                             ArmLearnWrapper& armLE,
                             Learn::LearningParameters params,   // copie : on ajuste localement
                             TrainingParameters trainingParams,  // copie
                             TPG::TPGExecutionEngineInstrumented& tee,
                             TPG::TPGGraph& tpgGraph,
                             const TPG::TPGFactoryInstrumented* factoryInstrumented,
                             TPG::ExecutionInfos& executionInfos);

/// Extrait tous les doubles d'un DataHandler.
std::vector<double> extractAllDoubles(const Data::DataHandler& handler);

/// Extrait et concatene l'etat courant complet du LE (tous ses DataSources).
std::vector<double> extractLEState(ArmLearnWrapper& armLE);

/// Ecrit le contenu de mapITI dans le header C LE_states.h (positions de depart
/// du LE : angle, vitesse, cible, ...) consomme par l'InferenceBenchmark embarque.
void storeToHeaderFile(const std::string& filename,
                       const std::map<std::list<int>, std::vector<TPG::InferenceTraceInfos>> mapITI,
                       const std::vector<DataSourceInfo>& dataSourcesInfo,
                       bool randomize,
                       TrainingParameters trainingParams,
                       bool minimalTeamCoverOnly);

/// Selects a minimal subset of graph traversals such that every Team
/// appearing in mapITI is visited by at least one kept traversal. 
std::map<std::list<int>, std::vector<TPG::InferenceTraceInfos>>
selectMinimalTeamCover(
    const std::map<std::list<int>, std::vector<TPG::InferenceTraceInfos>>& mapITI);

/// Affiche le contenu de mapITI (parcours -> nb d'occurences captures).
void print_mapITI(std::map<std::list<int>, std::vector<TPG::InferenceTraceInfos>> mapITI);

// =============================================================================
// MAIN
// =============================================================================
int main(int argc, char** argv)
{
    std::cout << "\033[1;33m=====[ codegen + export des etats du LE (rollout multi-actions) ]=====\033[0m"
              << std::endl;

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
    // Chargement du graphe (dot d'entrainement) dans un TPGGraph instrumente.
    // On part du graphe d'entrainement : c'est lui qu'on prune puis qu'on genere,
    // et c'est sur sa version prunee qu'on capture les etats -> une seule source
    // de verite pour codegen et capture (contrairement aux deux scripts separes).
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
    // -------------------------------------------------------------------------
    std::cout << "Play with TPG from the GEGELATI lib" << std::endl;
    float scoreGegelati; int nbActionsGegelati;
    validation(&scoreGegelati, &nbActionsGegelati, root, armLearnEnv, params, env,
               "tpg_gegelati_validation");

    // -------------------------------------------------------------------------
    // Pruning des elements non utilises + suppression des introns.
    // -------------------------------------------------------------------------
    ((const TPG::TPGFactoryInstrumented&)tpgGraph.getFactory()).clearUnusedTPGGraphElementsV2(tpgGraph);
    tpgGraph.clearProgramIntrons();
    root = tpgGraph.getRootVertices().front();
    armLearnEnv.resetIterations();

    // -------------------------------------------------------------------------
    // PHASE 2 - Verification du determinisme apres pruning (memes rollouts).
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
    // PHASE 3 - Capture des etats du LE le long de rollouts MULTI-ACTIONS.
    //
    // C'est ici que se joue le correctif : au lieu d'executer une seule action
    // par seed, on rejoue des episodes multi-actions (comme le pruning) sur le
    // graphe deja prune, en capturant (etat, parcours) a chaque pas. Les etats
    // profonds - et donc les parcours qui n'apparaissaient jamais - sont captures.
    // -------------------------------------------------------------------------
    std::cout << "\n\033[1;33m=====[ capture des etats du LE (rollout multi-actions, graphe prune) ]=====\033[0m"
              << std::endl;

    // Moteur d'execution instrumente + factory (pour isoler chaque parcours)
    TPG::TPGExecutionEngineInstrumented tee(env);
    const TPG::TPGFactoryInstrumented* factoryInstrumented =
        dynamic_cast<const TPG::TPGFactoryInstrumented*>(&tpgGraph.getFactory());
    if (!factoryInstrumented) {
        throw std::runtime_error("Error: TPGFactory is not of type TPGFactoryInstrumented.");
    }
    factoryInstrumented->resetTPGGraphCounters(tpgGraph);

    // Annotation du graphe pruné pour identifier les Teams lors des parcours.
    // Les identifiants sont assignes sur le graphe final -> ils correspondent au
    // graphe genere par la codegen.
    TPG::ExecutionInfos executionInfos;
    executionInfos.assignIdentifiers((const TPG::TPGTeamInstrumented*)root);
    std::cout << "completed assignIdentifiers()" << std::endl;

    // Rollouts multi-actions + equilibrage des classes de parcours
    std::map<std::list<int>, std::vector<TPG::InferenceTraceInfos>> mapITI =
        captureLEStatesAlongRollouts(root, armLearnEnv, params, trainingParams,
                                     tee, tpgGraph, factoryInstrumented, executionInfos);

    if (minimalTeamCoverOnly) {
        mapITI = selectMinimalTeamCover(mapITI);
    }

    // Etat final
    std::cout << "\n\033[1;34m----- Etat final de mapITI -----\033[0m\n";
    print_mapITI(mapITI);

    // Export du header d'etats du LE + JSON d'infos
    std::filesystem::create_directories("outLogs/precalcul");
    storeToHeaderFile("outLogs/precalcul/LE_states.h", mapITI,
                      armLearnEnv.getDataSourcesInfo(), randomizeSeeds, trainingParams, 
                      minimalTeamCoverOnly);

    // Reconstruit executionInfos a partir des valeurs conservees pour l'export JSON
    executionInfos.clear();
    std::vector<TPG::InferenceTraceInfos> overallInfTraceInfos;
    for (auto it = mapITI.begin(); it != mapITI.end(); ++it) {
        const std::vector<TPG::InferenceTraceInfos>& iTI = it->second;
        std::copy(iTI.begin(), iTI.end(), std::back_inserter(overallInfTraceInfos));
    }
    executionInfos.setVecInferenceTraceInfos(overallInfTraceInfos);
    executionInfos.writeTPGtoJson("outLogs/precalcul/tpgInfos.json");
    executionInfos.writeInfosToJson("outLogs/precalcul/executionInfos.json");

    // -------------------------------------------------------------------------
    // Suite de la codegen (inchangee) : stats, export .dot, comparaison,
    // testing du meilleur root, puis generation du code C.
    // -------------------------------------------------------------------------

    // Stats sur le graphe (necessaire pour la taille de pile requise)
    std::cout << "Analyze graph." << std::endl;
    TPG::PolicyStats ps;
    ps.setEnvironment(env);
    ps.analyzePolicy(tpgGraph.getRootVertices().front());
    std::ofstream bestStats;
    std::string bestPolicyStatsPath = codeGenPath + "best_root_pruned_stats.md";
    bestStats.open(bestPolicyStatsPath);
    bestStats << ps;
    bestStats.close();

    // Export du .dot prune
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

    // Generation du code C
    std::cout << "Printing C code." << std::endl;
    CodeGen::TPGGenerationEngineFactory factory(CodeGen::TPGGenerationEngineFactory::gotoMode);
    std::unique_ptr<CodeGen::TPGGenerationEngine> tpggen =
        factory.create("TPG", tpgGraph, codeGenPath,
                       trainingParams.instrType, codeGenParams.isInstrumented,
                       codeGenParams.isDecorated);
    tpggen->generateTPGGraph();

    std::cout << "End program" << std::endl;
    return 0;
}

// =============================================================================
// PHASE 1 & 2 : rollout multi-actions de validation (identique au codegen)
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

    while (nbEpisodes < params.nbIterationsPerPolicyEvaluation) {
        if (armLearnEnv.isTerminal() || nbActionsEp == params.maxNbActionsPerEval || *nbActions == 0) {
            *score += armLearnEnv.getScore();
            armLearnEnv.reset(0, Learn::LearningMode::VALIDATION);
            nbEpisodes++;
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

// =============================================================================
// PHASE 3 : capture des etats du LE le long de rollouts MULTI-ACTIONS
// =============================================================================
std::map<std::list<int>, std::vector<TPG::InferenceTraceInfos>>
captureLEStatesAlongRollouts(const TPG::TPGVertex* root,
                             ArmLearnWrapper& armLE,
                             Learn::LearningParameters params,
                             TrainingParameters trainingParams,
                             TPG::TPGExecutionEngineInstrumented& tee,
                             TPG::TPGGraph& tpgGraph,
                             const TPG::TPGFactoryInstrumented* factoryInstrumented,
                             TPG::ExecutionInfos& executionInfos)
{
    std::map<std::list<int>, std::vector<TPG::InferenceTraceInfos>> mapITI;

    int nbSeedsToSearch = DEFAULT_NB_SEEDS_TO_SEARCH;
    int continue_search = 0;
    int nbSeedsTried = 0;

    // Identifiant unique par echantillon capture. Indispensable : plusieurs pas
    // d'un meme episode produisent des etats DIFFERENTS ; leur donner le meme
    // "seed" ferait rejeter ces etats profonds par la detection de collision.
    int globalSampleId = 0;
    int globalVar = 0; // pour debug : print dataSourcesLE une seule fois

    do {
        std::cout << "\n\033[1;33m----- Nouvelle serie de recherche sur " << nbSeedsToSearch
                  << " trajectoires -----\033[0m" << std::endl;

        // Genere de nouvelles trajectoires de test (variete des conditions initiales)
        trainingParams.doTrainingValidation = true;
        params.nbIterationsPerPolicyEvaluation = nbSeedsToSearch;
        if (trainingParams.doTrainingValidation) {
            armLE.updateTrainingValidationTrajectories(params.nbIterationsPerPolicyEvaluation);
        }

        for (int j = 0; j < nbSeedsToSearch; j++) {
            // Etat initial de l'episode a partir de la trajectoire j
            armLE.reset(j, Learn::LearningMode::TESTING);

            int nbActionsEp = 0;

            // -----------------------------------------------------------------
            // ROLLOUT MULTI-ACTIONS : coeur du correctif.
            // On enchaine plusieurs actions (comme la validation/pruning) au lieu
            // d'une seule. A chaque pas on capture l'etat du LE ET le parcours de
            // graphe emprunte. Les etats profonds - atteignables uniquement apres
            // plusieurs interactions - sont ainsi captures, ce qui rend la
            // couverture des parcours complete.
            // -----------------------------------------------------------------
            while (!armLE.isTerminal() && nbActionsEp < params.maxNbActionsPerEval) {

                // 1) Capturer l'etat courant du LE (AVANT l'action)
                std::vector<double> dataSourcesLE = extractLEState(armLE);
                if (globalVar < 10){
                    // print all dataSourcesLE
                    std::cout << "dataSourcesLE: ";
                    for (const auto& val : dataSourcesLE) 
                        std::cout << val << " ";
                    std::cout << std::endl;
                }

                // 2) Isoler l'instrumentation pour lire le parcours de CE pas
                factoryInstrumented->resetTPGGraphCounters(tpgGraph);

                // 3) Executer une inference (un parcours racine -> action)
                auto trace = tee.executeFromRoot(*root);

                // 4) Enregistrer (parcours, etat) avec un id unique par echantillon
                executionInfos.analyzeExecution(tee, tpgGraph, globalSampleId++, dataSourcesLE);

                // 5) Appliquer l'action pour faire evoluer le LE vers l'etat suivant
                uint64_t actionID = ((TPG::TPGAction*)(trace.first.back()))->getActionID();
                armLE.doAction((double)actionID);

                if (globalVar < 10){
                    std::vector<double> dataSourcesLE = extractLEState(armLE);
                    // print all dataSourcesLE
                    std::cout << "dataSourcesLE: ";
                    for (const auto& val : dataSourcesLE) 
                        std::cout << val << " ";
                    std::cout << std::endl;
                    globalVar++;
                }

                nbActionsEp++;
            }
        }

        // Fin de serie : verser les captures dans la map equilibree
        std::vector<TPG::InferenceTraceInfos> vecInferenceTraceInfos =
            executionInfos.getVecInferenceTraceInfos();

        for (const TPG::InferenceTraceInfos& infTraceInfos : vecInferenceTraceInfos) {
            // Cle = parcours de graphe (list<int> traceTeamIds)
            if (!mapITI.count(infTraceInfos.traceTeamIds)) {
                mapITI.insert({infTraceInfos.traceTeamIds,
                               std::vector<TPG::InferenceTraceInfos>{infTraceInfos}});
            } else if (mapITI[infTraceInfos.traceTeamIds].size() < NB_VALUES_PER_CLASS) {
                // Verif de collision (ids uniques -> normalement jamais declenchee)
                bool collision = false;
                for (const TPG::InferenceTraceInfos& iTI : mapITI[infTraceInfos.traceTeamIds]) {
                    if (infTraceInfos.seed == iTI.seed) { collision = true; break; }
                }
                if (!collision) {
                    mapITI[infTraceInfos.traceTeamIds].push_back(infTraceInfos);
                } else {
                    std::cerr << "collision" << std::endl;
                }
            }
            // sinon : classe pleine, on ignore
        }

        std::cout << "\nStatus of mapITI after this round:\n";
        print_mapITI(mapITI);

        // Map equilibree ? (NB_VALUES_PER_CLASS occurences pour chaque parcours)
        int balanced = 1;
        for (auto it = mapITI.begin(); it != mapITI.end(); ++it) {
            if (it->second.size() < NB_VALUES_PER_CLASS) { balanced = 0; break; }
        }

        // On continue tant que non equilibre, dans la limite de MAX_NB_SEEDS_TO_SEARCH
        continue_search = !balanced && (nbSeedsTried < MAX_NB_SEEDS_TO_SEARCH);
        nbSeedsTried += nbSeedsToSearch;
        std::cout << "\rSeeds tried: " << nbSeedsTried << std::flush;

        executionInfos.clear();

    } while (continue_search);

    std::cout << "\ntotal seeds searched: " << nbSeedsTried << std::endl;
    std::cout << "graph traversal: " << mapITI.size() << std::endl;

    // Si on est sorti par MAX_NB_SEEDS_TO_SEARCH, certaines classes peuvent avoir
    // moins de NB_VALUES_PER_CLASS valeurs. On complete par duplication de la
    // derniere valeur pour garder un dataset equilibre (fallback).
    for (auto& [traceTeamIds, infosVec] : mapITI) {
        while (infosVec.size() < NB_VALUES_PER_CLASS) {
            if (!infosVec.empty()) {
                infosVec.push_back(infosVec.back());
            } else {
                std::cerr << "Warning: Unable to duplicate InferenceTraceInfos for empty vector."
                          << std::endl;
                break;
            }
        }
    }

    return mapITI;
}

// =============================================================================
// Helpers
// =============================================================================
void print_mapITI(std::map<std::list<int>, std::vector<TPG::InferenceTraceInfos>> mapITI)
{
    for (const auto& [traceTeams, vecITI] : mapITI) {
        std::cout << "[";
        bool first = true;
        for (int t : traceTeams) {
            if (!first) std::cout << " -> ";
            std::cout << "T" << t;
            first = false;
        }
        std::cout << "] : " << vecITI.size() << " / " << NB_VALUES_PER_CLASS << std::endl;
    }
    std::cout << "Total traversals: " << mapITI.size() << std::endl;
}

std::vector<double> extractAllDoubles(const Data::DataHandler& handler)
{
    std::vector<double> result;
    size_t n = handler.getAddressSpace(typeid(const double));
    result.reserve(n);
    for (size_t i = 0; i < n; i++) {
        double value = *handler.getDataAt(typeid(const double), i).getSharedPointer<const double>();
        result.push_back(value);
    }
    return result;
}

std::vector<double> extractLEState(ArmLearnWrapper& armLE)
{
    std::vector<double> dataSourcesLE;
    std::vector<std::reference_wrapper<const Data::DataHandler>> dataHandlers = armLE.getDataSources();
    for (const auto& handlerRef : dataHandlers) {
        std::vector<double> extracted = extractAllDoubles(handlerRef.get());
        dataSourcesLE.insert(dataSourcesLE.end(), extracted.begin(), extracted.end());
    }
    return dataSourcesLE;
}

void storeToHeaderFile(const std::string& filename,
                       const std::map<std::list<int>, std::vector<TPG::InferenceTraceInfos>> mapITI,
                       const std::vector<DataSourceInfo>& dataSourcesInfo,
                       bool randomize,
                       TrainingParameters trainingParams,
                       bool minimalTeamCoverOnly)
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Collecte des donnees
    std::vector<std::vector<double>> dataSources; // [nbValues][NB_DATA_SOURCES]
    std::vector<unsigned int> seeds;
    std::vector<unsigned int> ids_graph_traversals;

    // Mapping des parcours en commentaires
    if (minimalTeamCoverOnly) {
        file << "// Minimal set of graph traversals covering every Team." << std::endl;
    } else {
        file << "// All discovered graph traversals." << std::endl;
    }
    file << "// ";
    file << "// ===== Graph Traversal Mapping =====\n";
    int id_GT = 0;
    for (const auto& [traceTeams, vecITI] : mapITI) {
        file << "// [" << id_GT << "] -> [";
        bool first = true;
        for (int t : traceTeams) {
            if (!first) file << " -> ";
            file << "T" << t;
            first = false;
        }
        file << "]\n";

        for (auto const& iti : vecITI) {
            dataSources.push_back(iti.dataSourcesLE);
            seeds.push_back(iti.seed);
            ids_graph_traversals.push_back(id_GT);
        }
        id_GT++;
    }
    file << "// ===================================\n\n";

    // Indices 0..nbValues-1
    size_t nbValues = dataSources.size();
    std::vector<size_t> indices(nbValues);
    std::iota(indices.begin(), indices.end(), 0);

    // Randomisation optionnelle de l'ordre
    if (randomize) {
        unsigned int seed = 0;
        std::mt19937 g(seed);
        std::shuffle(indices.begin(), indices.end(), g);
    }

    // Header
    file << "#ifndef SEEDS_H\n"
         << "#define SEEDS_H\n\n"
         << "#include \"../codegen/externHeader.h\"\n\n"
         << "#define NB_SEED " << nbValues << "\n"
         << "#define NB_VALUES_PER_CLASS " << NB_VALUES_PER_CLASS << "\n"
         << "#define NB_CLASSES " << nbValues / NB_VALUES_PER_CLASS << "\n\n";

    // Tableaux dataSourcesLE (un tableau par feature)
    size_t featureIdx = 0;
    for (const auto& info : dataSourcesInfo) {
        file << "// " << info.name << "\n";
        for (size_t i = 0; i < info.size; ++i, ++featureIdx) {
            file << "static const " << trainingParams.instrType
                 << " dataSourcesLE_" << featureIdx;

            // cette motorPos ne varie pas, donc on duplique
            if (featureIdx >= 13 && featureIdx <= 14) {
                file << "[1] = { ";
                file << convEnvToInf(dataSources[indices[0]][featureIdx]);
                file << " };\n";
            } else {
                file << "[NB_SEED] = {";
                for (size_t j = 0; j < indices.size(); j++) {
                    if (j > 0) file << ", ";
                    file << convEnvToInf(dataSources[indices[j]][featureIdx]);
                }
                file << "};\n";
            }
        }
    }
    file << "\n";

    // Seeds (indicatif)
    file << "static const uint32_t seeds[NB_SEED] = {";
    for (size_t i = 0; i < indices.size(); i++) {
        if (i > 0) file << ", ";
        file << seeds[indices[i]];
    }
    file << "};\n";

    // Ids des parcours de graphe
    file << "static const unsigned int ids_graph_traversals[NB_SEED] = {";
    for (size_t i = 0; i < indices.size(); i++) {
        if (i > 0) file << ", ";
        file << ids_graph_traversals[indices[i]];
    }
    file << "};\n";

    file << "\n#endif // SEEDS_H\n";
    file.close();

    if (randomize) std::cout << "Data order was randomized." << std::endl;
    std::cout << "Data written to " << filename << " successfully." << std::endl;
}

/// @brief Selects a minimal subset of graph traversals such that every Team
///        appearing in mapITI is visited by at least one kept traversal.
///
/// This is a set-cover problem (NP-hard), solved with the classic greedy
/// heuristic: repeatedly keep the traversal that covers the largest number of
/// still-uncovered Teams. Ties are broken by shorter traversal, then by key
/// order, so the result is deterministic across runs.
///
/// Use when you don't want to benchmark every traversal class, but only need
/// enough traversals to exercise each Team of the TPG at least once.
std::map<std::list<int>, std::vector<TPG::InferenceTraceInfos>>
selectMinimalTeamCover(
    const std::map<std::list<int>, std::vector<TPG::InferenceTraceInfos>>& mapITI)
{
    // 1) Universe: every Team reachable through the discovered traversals
    std::set<int> uncovered;
    for (const auto& [traceTeams, _] : mapITI)
        uncovered.insert(traceTeams.begin(), traceTeams.end());

    const size_t nbTeamsTotal = uncovered.size();

    // 2) Greedy cover
    std::map<std::list<int>, std::vector<TPG::InferenceTraceInfos>> minimalMap;

    while (!uncovered.empty()) {
        const std::list<int>* bestKey = nullptr;
        size_t bestGain = 0;

        for (const auto& [traceTeams, vecITI] : mapITI) {
            // already kept -> skip
            if (minimalMap.count(traceTeams)) continue;

            size_t gain = 0;
            for (int t : traceTeams)
                if (uncovered.count(t)) gain++;

            // strictly better gain, or equal gain with a shorter traversal
            if (gain > bestGain ||
                (gain == bestGain && gain > 0 && bestKey &&
                 traceTeams.size() < bestKey->size())) {
                bestGain = gain;
                bestKey  = &traceTeams;
            }
        }

        // No remaining traversal covers any uncovered Team.
        // Happens if a Team of the pruned graph was never traversed during
        // capture -> it cannot be covered. Report it rather than loop forever.
        if (!bestKey || bestGain == 0) {
            std::cerr << "\n\033[1;31mWarning: " << uncovered.size()
                      << " Team(s) cannot be covered by any captured traversal: ";
            for (int t : uncovered) std::cerr << "T" << t << " ";
            std::cerr << "\033[0m" << std::endl;
            break;
        }

        // Keep this traversal (with all its NB_VALUES_PER_CLASS samples)
        minimalMap[*bestKey] = mapITI.at(*bestKey);
        for (int t : *bestKey) uncovered.erase(t);
    }

    std::cout << "\n\033[1;34m----- Minimal Team cover -----\033[0m\n";
    std::cout << "Traversals kept: " << minimalMap.size() << " / "
              << mapITI.size() << std::endl;
    std::cout << "Teams covered:   " << (nbTeamsTotal - uncovered.size())
              << " / " << nbTeamsTotal << std::endl;
    print_mapITI(minimalMap);

    return minimalMap;
}