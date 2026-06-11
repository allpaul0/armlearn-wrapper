#include "ArmLearnWrapper.h"

// ---------------------------------------------------------------------------
// computeInput — reads device state, fills all data arrays
// ---------------------------------------------------------------------------
void ArmLearnWrapper::computeInput() {

    auto deviceState = DeviceLearner::getDeviceState();

    std::vector<uint16_t> newMotorPos;
    int idx = 0;
    for (auto& motorState : deviceState) {
        for (uint16_t value : motorState) {
            motorPos.setDataAt(typeid(double), idx++, static_cast<double>(value));
            newMotorPos.push_back(value);
        }
    }

    auto coords = converter->computeServoToCoord(newMotorPos)->getCoord();
    for (int i = 0; i < static_cast<int>(coords.size()); ++i) {
        cartesianHand.setDataAt  (typeid(double), i, coords[i]);
        cartesianTarget.setDataAt(typeid(double), i, currentTarget->getInput()[i]);
        cartesianDiff.setDataAt  (typeid(double), i, currentTarget->getInput()[i] - coords[i]);
    }
}

// ---------------------------------------------------------------------------
// getDataSources
// ---------------------------------------------------------------------------
std::vector<std::reference_wrapper<const Data::DataHandler>>
ArmLearnWrapper::getDataSources() {
    auto result = std::vector<std::reference_wrapper<const Data::DataHandler>>();
    result.emplace_back(cartesianTarget);
    result.emplace_back(cartesianHand);
    result.emplace_back(cartesianDiff);
    result.emplace_back(motorPos);
    if (params.actionSpeed) result.emplace_back(dataMotorSpeed);
    return result;
}

// ---------------------------------------------------------------------------
// doAction — discrete action dispatch
// ---------------------------------------------------------------------------
void ArmLearnWrapper::doAction(double actionID) {

    checkpointEnv = std::make_shared<std::chrono::time_point<
        std::chrono::system_clock, std::chrono::nanoseconds>>(
            std::chrono::system_clock::now());

    const double step = params.sizeAction;
    std::vector<double> motorAction;

    switch (static_cast<uint8_t>(actionID)) {
        case 0:  motorAction = { step,  0,     0,     0,    0, 0}; break;
        case 1:  motorAction = { 0,     step,  0,     0,    0, 0}; break;
        case 2:  motorAction = { 0,     0,     step,  0,    0, 0}; break;
        case 3:  motorAction = { 0,     0,     0,     step, 0, 0}; break;
        case 4:  motorAction = {-step,  0,     0,     0,    0, 0}; break;
        case 5:  motorAction = { 0,    -step,  0,     0,    0, 0}; break;
        case 6:  motorAction = { 0,     0,    -step,  0,    0, 0}; break;
        case 7:  motorAction = { 0,     0,     0,    -step, 0, 0}; break;
        case 8:
            motorAction = {0, 0, 0, 0, 0, 0};
            if (algoIsDeterministic && !params.actionSpeed) isMoving = false;
            break;
        case 9:  motorAction = {0, 0, 0, 0,  step, 0}; break;
        case 10: motorAction = {0, 0, 0, 0,  0,  step}; break;
        case 11: motorAction = {0, 0, 0, 0, -step, 0}; break;
        case 12: motorAction = {0, 0, 0, 0,  0, -step}; break;
        default: motorAction = {0, 0, 0, 0, 0, 0}; break;
    }

    executeAction(motorAction);
}

// ---------------------------------------------------------------------------
// doActionContinuous — SAC variant
// ---------------------------------------------------------------------------
void ArmLearnWrapper::doActionContinuous(std::vector<float> actions) {

    checkpointEnv = std::make_shared<std::chrono::time_point<
        std::chrono::system_clock, std::chrono::nanoseconds>>(
            std::chrono::system_clock::now());

    std::vector<double> motorAction;
    for (float a : actions)
        motorAction.push_back(std::round(params.sizeAction * a));
    motorAction.push_back(0.0);
    motorAction.push_back(0.0);

    executeAction(motorAction);
}

// ---------------------------------------------------------------------------
// executeAction — apply motor deltas, update state, compute reward
// ---------------------------------------------------------------------------
void ArmLearnWrapper::executeAction(std::vector<double> motorAction) {

    // ---- Speed-mode integration ----
    if (params.actionSpeed) {
        for (int i = 0; i < 6; ++i) {
            motorSpeed[i] += motorAction[i];
            if (i < 4) dataMotorSpeed.setDataAt(typeid(double), i, motorSpeed[i]);
        }
        motorAction = motorSpeed;
    }

    int  nbMotorMoving           = 0;
    bool givePenaltyUnavailable  = false;

    auto scaledOutput = device->scalePosition({0,0,0,0,0,0}, -M_PI, M_PI);

    // ---- Motor 0 (base rotation, full 360 or clamped) ----
    {
        double cur = *motorPos.getDataAt(typeid(double), 0).getSharedPointer<const double>();
        if (params.canDo360) {
            scaledOutput[0] = static_cast<uint16_t>(
                static_cast<int>(motorAction[0] + cur) % 4096);
        } else if (motorAction[0] + cur >= 2 && motorAction[0] + cur <= 4094) {
            scaledOutput[0] = static_cast<uint16_t>(motorAction[0] + cur);
            if (motorAction[0] != 0) ++nbMotorMoving;
        } else {
            scaledOutput[0] = static_cast<uint16_t>(cur);
            if (algoIsDeterministic) isMoving = false;
            givePenaltyUnavailable = true;
        }
    }

    // ---- Motors 1–3 (shoulder, elbow, wrist) ----
    for (int i = 1; i < 4; ++i) {
        double cur = *motorPos.getDataAt(typeid(double), i).getSharedPointer<const double>();
        if (motorAction[i] + cur >= 1025 && motorAction[i] + cur <= 3071) {
            scaledOutput[i] = static_cast<uint16_t>(motorAction[i] + cur);
            if (motorAction[i] != 0) ++nbMotorMoving;
        } else {
            scaledOutput[i] = static_cast<uint16_t>(cur);
            if (algoIsDeterministic) isMoving = false;
            givePenaltyUnavailable = true;
        }
    }

    // ---- Collision check ----
    if (params.realSimulation && collisionDetector.hasCollision(scaledOutput)) {
        if (params.killIfCollision) {
            if (++valKillCollision == 5) isMoving = false;
        }
        if (algoIsDeterministic) isMoving = false;
        givePenaltyUnavailable = true;
        // Roll back to current positions
        for (int i = 0; i < 4; ++i) {
            double cur = *motorPos.getDataAt(typeid(double), i).getSharedPointer<const double>();
            scaledOutput[i] = static_cast<uint16_t>(cur);
        }
    } else {
        valKillCollision = std::max(0, valKillCollision - 1);
    }

    // ---- Passthrough for hand servos (motors 4–5) ----
    {
        double cur4 = *motorPos.getDataAt(typeid(double), 4).getSharedPointer<const double>();
        double cur5 = *motorPos.getDataAt(typeid(double), 5).getSharedPointer<const double>();
        scaledOutput[4] = static_cast<uint16_t>((scaledOutput[4] - 511) + cur4);
        scaledOutput[5] = static_cast<uint16_t>((scaledOutput[5] - 256) + cur5);
    }

    // ---- Apply position ----
    auto validOutput = device->toValidPosition(scaledOutput);
    device->setPosition(validOutput);
    device->waitFeedback();

    computeInput();

    // ---- Distance ----
    auto cartCoords = converter->computeServoToCoord(getMotorsPos())->getCoord();
    distance = computeSquaredError(currentTarget->getInput(), cartCoords);

    ++nbActionsDone;

    // ---- Threshold counter ----
    const double range = isValidation ? params.rangeTarget
                                      : trajectoryMgr.getCurrentRangeTarget();
    if (distance < range) ++nbActionsInThreshold;
    else                  nbActionsInThreshold = 0;

    // ---- Terminal via threshold ----
    if (!params.reachingObjectives && (nbActionsInThreshold == 10 || !isMoving))
        terminal = true;

    // ---- Reward (delegated) ----
    RewardCalculator::StepContext ctx{
        distance, range,
        givePenaltyUnavailable, nbMotorMoving,
        static_cast<int>(nbActionsDone), nbMaxActions,
        isMoving, isCycling,
        algoIsDeterministic,
        motorSpeed
    };
    auto [stepReward, termNow] = rewardCalc.compute(ctx);
    reward  = stepReward;
    terminal = terminal || termNow;
    score  += reward;

    // ---- Deterministic score override (TPG) ----
    if (algoIsDeterministic) {
        score = rewardCalc.deterministicScore(
            distance, static_cast<int>(nbActionsDone), nbMaxActions,
            trajectoryMgr.getCurrentRangeTarget(),
            params.bonusNbIteration, isValidation);
    }

    // ---- Cycle detection ----
    if (algoIsDeterministic) updateAndCheckCycles();

    // ---- Recording ----
    if (params.testing) {
        double stepTime = std::chrono::duration<double>(
            std::chrono::system_clock::now() - *checkpointEnv).count();
        recorder.recordStep(getMotorsPos(), stepTime);
        if (terminal || nbActionsDone == static_cast<size_t>(nbMaxActions))
            recorder.endEpisode(score, distance, params.rangeTarget,
                                static_cast<int>(nbActionsDone));
    }

    timeEnv += std::chrono::duration<double>(
        std::chrono::system_clock::now() - *checkpointEnv).count();
}

// ---------------------------------------------------------------------------
// updateAndCheckCycles
// ---------------------------------------------------------------------------
void ArmLearnWrapper::updateAndCheckCycles() {
    auto pos = getMotorsPos();
    if (std::find(memoryMotorPos.begin(), memoryMotorPos.end(), pos)
            != memoryMotorPos.end()) {
        isCycling = true;
    } else {
        memoryMotorPos.push_back(pos);
    }
}

// ---------------------------------------------------------------------------
// reset
// ---------------------------------------------------------------------------
void ArmLearnWrapper::reset(size_t /*seed*/, Learn::LearningMode mode) {

    std::vector<EpisodeTrajectory>* trajectories;
    switch (mode) {
        case Learn::LearningMode::TRAINING:   trajectories = &trajectoryMgr.getTraining();           break;
        case Learn::LearningMode::VALIDATION: trajectories = &trajectoryMgr.getValidation();         break;
        case Learn::LearningMode::TESTING:    trajectories = &trajectoryMgr.getTrainingValidation(); break;
    }

    currentStartingPos = trajectories->at(iterationNb).first;
    currentTarget      = trajectories->at(iterationNb).second;

    device->setPosition(*currentStartingPos);
    device->waitFeedback();

    ++iterationNb;
    if (iterationNb >= static_cast<int>(trajectories->size())) iterationNb = 0;

    computeInput();

    // Reset episode state
    score              = 0.0;
    reward             = 0.0;
    distance           = 0.0;
    nbActionsDone      = 0;
    nbActionsInThreshold = 0;
    terminal           = false;
    isMoving           = true;
    isCycling          = false;
    valKillCollision   = 0;
    timeEnv            = 0.0;
    isValidation       = (mode == Learn::LearningMode::VALIDATION);
    motorSpeed         = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    memoryMotorPos.clear();

    // Recording setup
    if (params.testing) {
        const auto& sp  = *trajectories->at(iterationNb).first;
        const auto& tgt = trajectories->at(iterationNb).second->getInput();
        recorder.beginEpisode();
        recorder.setEpisodeHeader(sp, {tgt[0], tgt[1], tgt[2]});
    }
}

// ---------------------------------------------------------------------------
// Trivial LearningEnvironment overrides
// ---------------------------------------------------------------------------
double ArmLearnWrapper::getScore()   const { return score; }
bool   ArmLearnWrapper::isTerminal() const { return terminal; }
bool   ArmLearnWrapper::isCopyable() const { return true; }

Learn::LearningEnvironment* ArmLearnWrapper::clone() const {
    return new ArmLearnWrapper(*this);
}

// ---------------------------------------------------------------------------
// getMotorsPos
// ---------------------------------------------------------------------------
std::vector<uint16_t> ArmLearnWrapper::getMotorsPos() {
    std::vector<uint16_t> pos;
    for (auto& state : DeviceLearner::getDeviceState())
        for (uint16_t v : state)
            pos.push_back(v);
    return pos;
}

// ---------------------------------------------------------------------------
// toString / newGoalToString
// ---------------------------------------------------------------------------
std::string ArmLearnWrapper::toString() const {
    std::stringstream ss;
    for (int i = 0; i < 6; ++i)
        ss << *motorPos.getDataAt(typeid(double), i).getSharedPointer<const double>() << " ; ";
    ss << "    -->    ";
    for (int i = 0; i < 3; ++i)
        ss << *cartesianHand.getDataAt(typeid(double), i).getSharedPointer<const double>() << " ; ";
    ss << " - (goal: " << currentTarget->getInput()[0] << " ; "
                       << currentTarget->getInput()[1] << " ; "
                       << currentTarget->getInput()[2] << ")";
    return ss.str();
}

std::string ArmLearnWrapper::newGoalToString() const {
    std::stringstream ss;
    ss << " - (new goal: " << currentTarget->getInput()[0] << " ; "
                           << currentTarget->getInput()[1] << " ; "
                           << currentTarget->getInput()[2] << ")\n";
    return ss.str();
}

// ---------------------------------------------------------------------------
// getDataSourcesInfo
// ---------------------------------------------------------------------------
std::vector<DataSourceInfo> ArmLearnWrapper::getDataSourcesInfo() const {
    std::vector<DataSourceInfo> infos;
    infos.push_back({"cartesianTarget", cartesianTarget.getDimensionsSize().at(0)});
    infos.push_back({"cartesianHand",   cartesianHand.getDimensionsSize().at(0)});
    infos.push_back({"cartesianDiff",   cartesianDiff.getDimensionsSize().at(0)});
    infos.push_back({"motorPos",        motorPos.getDimensionsSize().at(0)});
    if (params.actionSpeed)
        infos.push_back({"dataMotorSpeed", dataMotorSpeed.getDimensionsSize().at(0)});
    return infos;
}