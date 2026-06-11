#include "TrajectoryManager.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <cmath>
#include <stdexcept>

// ---------------------------------------------------------------------------
// Construction / destruction
// ---------------------------------------------------------------------------

TrajectoryManager::TrajectoryManager(TrainingParameters&   params,
                                     IKinematicsConverter& converter,
                                     ICollisionDetector&   collisionDetector,
                                     Mutator::RNG&         rng,
                                     ValidatorFn           toValidPosition)
    : params(params),
      converter(converter),
      collisionDetector(collisionDetector),
      rng(rng),
      toValidPosition(std::move(toValidPosition)),
      initStartingPos(BACKHOE_POSITION)
{
    loadTargetCSV();

    if (params.progressiveRangeTarget) {
        currentRangeTarget = params.maxLengthTargets;
    } else {
        currentRangeTarget = params.rangeTarget;
        if (params.progressiveModeStartingPos)
            currentMaxLimitStartingPos = params.maxLengthStartingPos;
        if (params.progressiveModeTargets)
            currentMaxLimitTarget = params.maxLengthTargets;
    }
}

TrajectoryManager::TrajectoryManager(const TrajectoryManager& other)
    : params(other.params),
      converter(other.converter),
      collisionDetector(other.collisionDetector),
      rng(other.rng),
      toValidPosition(other.toValidPosition),
      initStartingPos(other.initStartingPos),
      dataTarget(other.dataTarget),
      scoreTrajectories(other.scoreTrajectories),
      currentMaxLimitTarget(other.currentMaxLimitTarget),
      currentMaxLimitStartingPos(other.currentMaxLimitStartingPos),
      currentRangeTarget(other.currentRangeTarget)
{
    // Deep-copy each trajectory set: targets are always heap-allocated;
    // starting positions are heap-allocated only when doRandomStartingPosition.
    auto deepCopy = [&](const std::vector<EpisodeTrajectory>& src,
                        std::vector<EpisodeTrajectory>&       dst,
                        bool copyStartingPos)
    {
        dst.reserve(src.size());
        for (const auto& t : src) {
            auto* sp  = copyStartingPos
                        ? new std::vector<uint16_t>(*t.first)
                        : t.first;     // shared pointer to initStartingPos — not owned
            auto* tgt = new armlearn::Input<double>(t.second->getInput());
            dst.push_back({sp, tgt});
        }
    };

    deepCopy(other.trainingTrajectories,           trainingTrajectories,           params.doRandomStartingPosition);
    deepCopy(other.validationTrajectories,         validationTrajectories,         params.doRandomStartingPosition);
    deepCopy(other.trainingValidationTrajectories, trainingValidationTrajectories, params.doRandomStartingPosition);
}

TrajectoryManager::~TrajectoryManager() {
    clearAll();
}

// ---------------------------------------------------------------------------
// EpisodeTrajectory generation — public
// ---------------------------------------------------------------------------

void TrajectoryManager::updateTraining(int nbTrajectories) {

    std::cout << "updateTrainingStart" << std::endl;
    clearPropTraining();
    while (static_cast<int>(trainingTrajectories.size()) < nbTrajectories) {
        std::cout << "bouh" << std::endl;
        auto* sp  = params.doRandomStartingPosition
                    ? generateRandomStartingPos(false)
                    : &initStartingPos;
        std::cout << "randomGoalStart" << std::endl;
        auto* tgt = randomGoal(*sp, false);
        std::cout << "randomGoalEnd" << std::endl;
        trainingTrajectories.push_back({sp, tgt});
    }
    std::cout << "updateTrainingEnd" << std::endl;
}

void TrajectoryManager::updateTrainingValidation(int nbTrajectories) {
    clearVector(trainingValidationTrajectories, /*deleteStartingPos=*/true);
    for (int i = 0; i < nbTrajectories; ++i) {
        auto* sp  = params.doRandomStartingPosition
                    ? generateRandomStartingPos(false)
                    : &initStartingPos;
        auto* tgt = randomGoal(*sp, false);
        trainingValidationTrajectories.push_back({sp, tgt});
    }
}

void TrajectoryManager::updateValidation(int nbTrajectories) {
    clearVector(validationTrajectories, /*deleteStartingPos=*/false);
    for (int i = 0; i < nbTrajectories; ++i) {
        auto* sp  = params.doRandomStartingPosition
                    ? generateRandomStartingPos(true)
                    : &initStartingPos;
        auto* tgt = randomGoal(*sp, true);
        validationTrajectories.push_back({sp, tgt});
    }
}

void TrajectoryManager::customTrajectory(armlearn::Input<double>*     newGoal,
                                         const std::vector<uint16_t>& startingPos,
                                         bool                         useValidation)
{
    // NOTE: The original code operated on a local copy of the vector so
    // all mutations were silently discarded. This version uses a reference.
    std::vector<EpisodeTrajectory>& trajectories =
        useValidation ? validationTrajectories : trainingTrajectories;

    if (!trajectories.empty()) {
        auto it = trajectories.begin();
        if (params.doRandomStartingPosition) delete it->first;
        delete it->second;
        trajectories.erase(it);
    }

    // Copy the starting position so the caller's stack frame can go away safely.
    trajectories.insert(trajectories.begin(),
                        {new std::vector<uint16_t>(startingPos), newGoal});
}

// ---------------------------------------------------------------------------
// Score tracking
// ---------------------------------------------------------------------------

void TrajectoryManager::addScore(int index, double score) {
    scoreTrajectories.push_back({index, score});
}

// ---------------------------------------------------------------------------
// Persistence
// ---------------------------------------------------------------------------

void TrajectoryManager::saveValidationTrajectories(const std::string& path) const {
    std::ofstream out(path);
    if (!out.is_open()) {
        std::cerr << "[TrajectoryManager] Cannot open " << path << " for writing.\n";
        return;
    }
    for (const auto& t : validationTrajectories) {
        for (auto v : *t.first) out << v << ' ';
        const auto& inp = t.second->getInput();
        out << inp[0] << ' ' << inp[1] << ' ' << inp[2] << '\n';
    }
}

void TrajectoryManager::loadValidationTrajectories(const std::string& path) {
    clearVector(validationTrajectories, /*deleteStartingPos=*/false);

    std::ifstream in(path);
    if (!in.is_open()) {
        std::cerr << "[TrajectoryManager] Cannot open " << path << " for reading.\n";
        return;
    }

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        std::vector<double> vals;
        double v;
        while (ss >> v) vals.push_back(v);
        if (vals.size() < 9) continue;

        auto* sp = new std::vector<uint16_t>();
        for (int i = 0; i < 6; ++i)
            sp->push_back(static_cast<uint16_t>(vals[i]));

        auto* tgt = new armlearn::Input<double>({vals[6], vals[7], vals[8]});
        validationTrajectories.push_back({sp, tgt});
    }
}

// ---------------------------------------------------------------------------
// Internal helpers — memory management
// ---------------------------------------------------------------------------

void TrajectoryManager::clearAll() {
    clearVector(trainingTrajectories,           /*deleteStartingPos=*/true);
    clearVector(validationTrajectories,         /*deleteStartingPos=*/false);
    clearVector(trainingValidationTrajectories, /*deleteStartingPos=*/true);
}

void TrajectoryManager::clearVector(std::vector<EpisodeTrajectory>& v, bool deleteStartingPos) {
    for (auto& t : v) {
        if (deleteStartingPos && params.doRandomStartingPosition) delete t.first;
        delete t.second;
    }
    v.clear();
}

void TrajectoryManager::clearPropTraining() {
    if (trainingTrajectories.empty())            return;
    if (params.propTrajectoriesReused >= 1.0)    return;

    const int nbToDelete = static_cast<int>(
        std::round(trainingTrajectories.size() * (1.0 - params.propTrajectoriesReused)));

    // 1. Delete and erase the front slice unconditionally
    {
        auto it = trainingTrajectories.begin();
        std::advance(it, nbToDelete);
        for (auto jt = trainingTrajectories.begin(); jt != it; ++jt) {
            if (params.doRandomStartingPosition) delete jt->first;
            delete jt->second;
        }
        trainingTrajectories.erase(trainingTrajectories.begin(), it);
    }

    // 2. Score-guided deletion of a further nbToDelete entries from the remainder
    std::sort(scoreTrajectories.begin(), scoreTrajectories.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    std::vector<int> toDelete;
    toDelete.reserve(nbToDelete);
    for (int i = 0; i < nbToDelete && i < static_cast<int>(scoreTrajectories.size()); ++i) {
        toDelete.push_back(params.controlTrajectoriesDeletion
                           ? scoreTrajectories[i].first
                           : i);
    }

    // Iterate backward to keep indices stable during erasure
    for (int i = static_cast<int>(trainingTrajectories.size()) - 1; i >= 0; --i) {
        if (std::find(toDelete.begin(), toDelete.end(), i) != toDelete.end()) {
            auto it = trainingTrajectories.begin();
            std::advance(it, i);
            if (params.doRandomStartingPosition) delete it->first;
            delete it->second;
            trainingTrajectories.erase(it);
        }
    }

    scoreTrajectories.clear();
}

// ---------------------------------------------------------------------------
// Internal helpers — CSV loading
// ---------------------------------------------------------------------------

void TrajectoryManager::loadTargetCSV(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[TrajectoryManager] Cannot open " << path << '\n';
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::vector<double> vals;
        std::stringstream   ss(line);
        std::string         tok;
        while (std::getline(ss, tok, ',')) {
            std::istringstream iss(tok);
            double d;
            if (iss >> d) vals.push_back(d);
        }
        if (!vals.empty()) dataTarget.push_back(std::move(vals));
    }
    // Shuffle once so every run with the same seed sees the same order
    std::mt19937 rngData(params.seed);
    std::shuffle(dataTarget.begin(), dataTarget.end(), rngData);
}

// ---------------------------------------------------------------------------
// Internal helpers — random position / goal generation
// ---------------------------------------------------------------------------

std::vector<uint16_t>* TrajectoryManager::generateRandomStartingPos(bool validation) {
    std::vector<uint16_t> motorPositions;
    bool   invalid;
    size_t chosenIndex;

    do {
        chosenIndex          = rng.getUnsignedInt64(0, dataTarget.size());
        auto cartGoal        = dataTarget[chosenIndex];

        // In training with a distance cap, keep resampling until the goal is
        // close enough to the home position
        while (!validation && !params.progressiveModeMotor &&
               ArmLearnUtils::squaredError(
                   converter.servoToCartesian(initStartingPos), cartGoal)
               > currentMaxLimitStartingPos)
        {
            chosenIndex = rng.getUnsignedInt64(0, dataTarget.size());
            cartGoal    = dataTarget[chosenIndex];
        }

        motorPositions = randomMotorPos(cartGoal, validation, /*isTarget=*/false);

        invalid = params.realSimulation && collisionDetector.hasCollision(motorPositions);
        if (invalid)
            dataTarget.erase(dataTarget.begin() + chosenIndex);

    } while (invalid);

    return new std::vector<uint16_t>(motorPositions);
}

armlearn::Input<double>* TrajectoryManager::randomGoal(
    const std::vector<uint16_t>& startingPos, bool validation)
{
    std::vector<uint16_t> motorPos;
    std::vector<double>   cartCoords;
    bool distBad, handBad;
    size_t index;

    do {
        std::cout << "do" <<std::endl;
        index        = rng.getUnsignedInt64(0, dataTarget.size());
        auto cartGoal = dataTarget[index];

        motorPos   = randomMotorPos(cartGoal, validation, /*isTarget=*/true);
        cartCoords = converter.servoToCartesian(motorPos);

        const double dist = ArmLearnUtils::squaredError(
            converter.servoToCartesian(startingPos), cartCoords);

        distBad = (dist > currentMaxLimitTarget || dist < currentRangeTarget);
        handBad = params.realSimulation && collisionDetector.hasCollision(motorPos);

        if (handBad){
            // Delete index because it will never be correct
            auto it = dataTarget.begin();
            std::advance(it, index);
            dataTarget.erase(it);
        }

    } while ((!validation && distBad && !params.progressiveModeMotor) || handBad);

    return new armlearn::Input<double>({cartCoords[0], cartCoords[1], cartCoords[2]});
}

std::vector<uint16_t> TrajectoryManager::randomMotorPos(
    const std::vector<double>& cartGoal, bool validation, bool isTarget)
{
    const double  limit = isTarget ? currentMaxLimitTarget : currentMaxLimitStartingPos;
    const int16_t vn    = static_cast<int16_t>(2048 % static_cast<int>(params.sizeAction));
    const int     sa    = static_cast<int>(params.sizeAction);

    double                minDist = 1e4;
    std::vector<uint16_t> best;

    for (int attempt = 0; attempt < 10000; ++attempt) {
        uint16_t i, j, k, l;

        if (!validation && params.progressiveModeMotor) {
            // Sample within [2048 ± limit], snapped to grid
            auto clampSample = [&](double lo, double hi) -> uint16_t {
                return static_cast<uint16_t>(
                    vn + (rng.getInt32(static_cast<int32_t>(lo - vn),
                                      static_cast<int32_t>(hi - vn)) / sa) * sa);
            };
            i = clampSample(std::max(2.0,    2048.0 - limit), std::min(4094.0, 2048.0 + limit));
            j = clampSample(std::max(1025.0, 2048.0 - limit), std::min(3071.0, 2048.0 + limit));
            k = clampSample(std::max(1025.0, 2048.0 - limit), std::min(3071.0, 2048.0 + limit));
            l = clampSample(std::max(1025.0, 2048.0 - limit), std::min(3071.0, 2048.0 + limit));
        } else {
            // Sample over full valid range, snapped to grid
            i = static_cast<uint16_t>(vn + (rng.getInt32(1    - vn, 4096 - vn) / sa) * sa);
            j = static_cast<uint16_t>(vn + (rng.getInt32(1025 - vn, 3071 - vn) / sa) * sa);
            k = static_cast<uint16_t>(vn + (rng.getInt32(1025 - vn, 3071 - vn) / sa) * sa);
            l = static_cast<uint16_t>(vn + (rng.getInt32(1025 - vn, 3071 - vn) / sa) * sa);
        }

        // Clamp to device physical limits
        std::vector<uint16_t> candidate = toValidPosition({i, j, k, l, 512, 256});

        // In progressive Cartesian mode return the first valid candidate immediately
        if (!validation && params.progressiveModeTargets && !params.progressiveModeMotor)
            return candidate;

        const double d = ArmLearnUtils::squaredError(
            converter.servoToCartesian(candidate), cartGoal);

        if (d < minDist) {
            minDist = d;
            best    = candidate;
        }
    }
    return best;
}