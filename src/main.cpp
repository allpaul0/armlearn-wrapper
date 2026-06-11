#include <unordered_set>
#include <string>
#include <atomic>
#include <cfloat>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <iostream>

#include <gegelati.h>
#include "instructions.h"
#include "params/trainingParameters.h"
#include "armlearn/logger/ArmLearnLogger.h"
#include "armlearn/environment/ArmLearnWrapper.h"
#include "armlearn/training/ArmLearningAgent.h"
// New headers required by the refactored constructors
#include "armlearn/evaluation/EvaluationPolicy.h"
#include "armlearn/training/CurriculumManager.h"


void getKey(std::atomic<bool>& exit) {
    std::cout << std::endl;
    std::cout << "Press `q` then [Enter] to exit." << std::endl;
    std::cout.flush();

    exit = false;

    while (!exit) {
        char c;
        std::cin >> c;
        switch (c) {
        case 'q':
        case 'Q':
            exit = true;
            break;
        default:
            printf("Invalid key '%c' pressed.", c);
            std::cout.flush();
        }
    }

    printf("Program will terminate at the end of next generation.\n");
    std::cout.flush();
}

int main() {
    std::cout << "Start ArmLearner application." << std::endl;

    TrainingParameters trainingParams;
    trainingParams.loadParametersFromJson("params/trainParams.json");

    // Set the parameters for the learning process.
    Learn::LearningParameters params;
    File::ParametersParser::loadParametersFromJson("params/params.json", params);

    // Create the instruction set for programs
    Instructions::Set gpis;
    fillInstructionSet(gpis, trainingParams);

    // Instantiate the LearningEnvironment
    ArmLearnWrapper armLearnEnv(params.maxNbActionsPerEval, trainingParams, true);

    // Prompt the number of threads
    std::cout << "Number of threads: " << params.nbThreads << std::endl;

    // Generate validation targets.
    if (params.doValidation && !trainingParams.loadValidationTrajectories) {
        armLearnEnv.updateValidationTrajectories(params.nbIterationsPerPolicyEvaluation);
    }

    if (trainingParams.doTrainingValidation) {
        armLearnEnv.updateTrainingValidationTrajectories(params.nbIterationsPerPolicyEvaluation);
    }

    // -------------------------------------------------------------------------
    // Instantiate the evaluation policy and (optionally) the curriculum manager.
    //
    // StandardEvaluationPolicy encapsulates the early-cancellation and score-
    // aggregation logic that previously lived inline in ArmLearningAgent::evaluateJob().
    //
    // CurriculumManager is only wired in when progressive training is active;
    // otherwise a nullptr is passed and the agent skips curriculum updates.
    // -------------------------------------------------------------------------
    Learn::StandardEvaluationPolicy evalPolicy(trainingParams);

    const bool doUpdateLimits =
        trainingParams.progressiveModeTargets || trainingParams.progressiveModeStartingPos;

    std::unique_ptr<CurriculumManager> curriculum;
    if (doUpdateLimits) {
        curriculum = std::make_unique<CurriculumManager>(
            trainingParams, armLearnEnv.getTrajectoryManager());
    }

    // Instantiate and init the learning agent
    Learn::ArmLearningAgent la(
        armLearnEnv, gpis, params, trainingParams,
        evalPolicy,
        curriculum.get());   // nullptr when progressive training is off

    la.init(trainingParams.seed);

    std::atomic<bool> exitProgram = false;
    std::thread threadKeyboard;

    if (trainingParams.interactiveMode && !trainingParams.testing) {
#ifndef NO_CONSOLE_CONTROL
        threadKeyboard = std::thread(getKey, std::ref(exitProgram));
        while (exitProgram);
#else
        std::atomic<bool> exitProgram = false;
#endif
    }

    // -------------------------------------------------------------------------
    // Loggers
    //
    // ArmLearnLogger no longer takes a LearningAgent reference — it only needs
    // the four boolean flags that control which columns are printed.
    // Loggers must be registered with the agent via addLogger().
    // -------------------------------------------------------------------------
    const bool doTrainingValidation = trainingParams.doTrainingValidation && doUpdateLimits;

    std::string nameLogs = (!!trainingParams.testing) ? "logsGegelati" : "garbage";
    std::ofstream fichier(("outLogs/" + nameLogs + ".ods"), std::ios::out);

    // LALogger's constructor registers itself with `la` automatically.
    // doValidation is read from la.params.doValidation inside LALogger; we
    // pass the remaining arm-specific flags as extra arguments.
    Log::ArmLearnLogger logFile(
        la,
        doTrainingValidation,
        doUpdateLimits,
        trainingParams.controlTrajectoriesDeletion,
        fichier);

    Log::ArmLearnLogger logCout(
        la,
        doTrainingValidation,
        doUpdateLimits,
        trainingParams.controlTrajectoriesDeletion);

    // Use previous Graphs
    if (trainingParams.startPreviousTPG) {
        auto& tpg = *la.getTPGGraph();
        Environment env(gpis, params, armLearnEnv.getDataSources());
        File::TPGGraphDotImporter dotImporter(
            ("outLogs/dotfiles/" + trainingParams.namePreviousTPG).c_str(), env, tpg);
    }

    // Save / load validation trajectories
    if (trainingParams.saveValidationTrajectories) {
        armLearnEnv.saveValidationTrajectories();
    }
    if (trainingParams.loadValidationTrajectories) {
        armLearnEnv.loadValidationTrajectories();
    }

    if (trainingParams.testing) {
        auto& tpg = *la.getTPGGraph();
        Environment env(gpis, params, armLearnEnv.getDataSources());
        File::TPGGraphDotImporter dotImporter(
            (trainingParams.tpgDotPathTraining + "/best_root.dot").c_str(), env, tpg);
        la.testingBestRoot(params.nbIterationsPerPolicyEvaluation);
    } else {

        std::ofstream stats;
        stats.open("outLogs/bestPolicyStats.md");
        Log::LAPolicyStatsLogger logStats(la, stats);

        File::TPGGraphDotExporter dotExporter("outLogs/dotfiles/out_0000.dot", *la.getTPGGraph());

        auto checkpoint = std::make_shared<std::chrono::time_point<
            std::chrono::system_clock, std::chrono::nanoseconds>>(
                std::chrono::system_clock::now());
        bool timeLimitReached = false;

        std::cout << "start training" << std::endl;

        for (uint64_t i = 0; i < params.nbGenerations && !exitProgram && !timeLimitReached; i++) {

            armLearnEnv.updateTrainingTrajectories(trainingParams.nbIterationTraining);

            std::cout << "updateTrainingTrajectories" << std::endl;

            char buff[64];
            sprintf(buff, "outLogs/dotfiles/out_%04d.dot", static_cast<uint16_t>(i));
            dotExporter.setNewFilePath(buff);
            dotExporter.print();

            std::cout << "dotExporter.print" << std::endl;

            la.trainOneGeneration(i);

            if (trainingParams.timeMaxTraining > 0) {
                timeLimitReached = (
                    ((std::chrono::duration<double>)(
                        std::chrono::system_clock::now() - *checkpoint)).count()
                    > trainingParams.timeMaxTraining);
            }
        }

        la.keepBestPolicy();
        dotExporter.setNewFilePath("outLogs/best_root.dot");
        dotExporter.print();

        TPG::PolicyStats ps;
        ps.setEnvironment(la.getTPGGraph()->getEnvironment());
        ps.analyzePolicy(la.getBestRoot().first);
        std::ofstream bestStats;
        bestStats.open("outLogs/best_root_stats.md");
        bestStats << ps;
        bestStats.close();

        stats.close();
    }

    // cleanup
    for (unsigned int i = 0; i < gpis.getNbInstructions(); i++) {
        delete (&gpis.getInstruction(i));
    }

    if (trainingParams.interactiveMode && !trainingParams.testing) {
#ifndef NO_CONSOLE_CONTROL
        std::cout << "Exiting program, press a key then [enter] to exit if nothing happens.";
        threadKeyboard.join();
#endif
    }

    return 0;
}