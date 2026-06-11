#ifndef EPISODE_RECORDER_H
#define EPISODE_RECORDER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <memory>
#include <cstdint>

/**
 * @brief Records per-step motor positions and per-episode metadata for the
 * testing/evaluation export pipeline.
 *
 * Previously this logic was interleaved with ArmLearnWrapper::saveMotorPos()
 * and ArmLearnWrapper::logTestingTrajectories(). Keeping it here makes it
 * possible to enable/disable recording without touching the environment logic,
 * and makes the CSV format easy to change in one place.
 */
class EpisodeRecorder {
public:

    // -----------------------------------------------------------------------
    // Episode lifecycle
    // -----------------------------------------------------------------------

    /** Call at the beginning of each episode (reset). */
    void beginEpisode() {
        stepPositions.clear();
        currentEpisodeHeader.clear();
        episodeCheckpoint = std::make_shared<Clock>(std::chrono::system_clock::now());
        accumulatedEnvTime = 0.0;
    }

    /** Store the trajectory header: starting motor positions + Cartesian target. */
    void setEpisodeHeader(const std::vector<uint16_t>& startPos,
                          const std::vector<double>&   target)
    {
        currentEpisodeHeader.clear();
        for (auto v : startPos) currentEpisodeHeader.push_back(static_cast<int32_t>(v));
        for (double t : target)  currentEpisodeHeader.push_back(static_cast<int32_t>(t));
    }

    /** Record motor positions after each step. */
    void recordStep(const std::vector<uint16_t>& motorPos, double stepEnvTime) {
        stepPositions.push_back(motorPos);
        accumulatedEnvTime += stepEnvTime;
    }

    /**
     * @brief Finalise the current episode and store its summary.
     *
     * @param score         Episode score (will be stored ×1000 as int32)
     * @param distance      Final distance (×1000 as int32)
     * @param rangeTarget   Threshold below which the episode counts as success
     * @param nbActionsDone Number of actions taken
     */
    void endEpisode(double score, double distance, double rangeTarget, int nbActionsDone) {
        if (!episodeCheckpoint) return;

        std::vector<int32_t> row = currentEpisodeHeader;

        const double elapsed = std::chrono::duration<double>(
            std::chrono::system_clock::now() - *episodeCheckpoint).count()
            - accumulatedEnvTime;

        row.push_back(static_cast<int32_t>(elapsed * 1e6));          // duration µs
        row.push_back(static_cast<int32_t>(score    * 1000.0));       // score ×1000
        row.push_back(static_cast<int32_t>(distance * 1000.0));       // distance ×1000
        row.push_back(static_cast<int32_t>(distance < rangeTarget ? 1 : 0)); // success flag
        row.push_back(static_cast<int32_t>(nbActionsDone));

        for (const auto& mp : stepPositions) {
            row.push_back(mp[0]);
            row.push_back(mp[1]);
            row.push_back(mp[2]);
            row.push_back(mp[3]);
        }

        allEpisodes.push_back(std::move(row));
    }

    // -----------------------------------------------------------------------
    // Export
    // -----------------------------------------------------------------------

    /**
     * @brief Write all recorded episodes to a CSV file and clear the buffer.
     *
     * @param useGegelati  Selects the output filename suffix.
     * @param exportDir    Directory where the file is written (no trailing /).
     */
    void exportCSV(bool useGegelati, const std::string& exportDir) {
        const std::string fileName = exportDir
            + (useGegelati ? "/outputGegelati.csv" : "/outputSAC.csv");

        std::ofstream out(fileName);
        if (!out.is_open()) {
            std::cerr << "[EpisodeRecorder] Cannot open " << fileName << "\n";
            return;
        }

        out << "armPos0,armPos1,armPos2,armPos3,armPos4,armPos5,"
               "targetPos0,targetPos1,targetPos2,"
               "Duration(ms),Score,Distance,Success,NbActions,MotorPos\n";

        for (const auto& row : allEpisodes) {
            for (size_t i = 0; i < row.size(); ++i) {
                out << row[i];
                if (i + 1 < row.size()) out << ',';
            }
            out << '\n';
        }

        allEpisodes.clear();
    }

    bool hasData() const { return !allEpisodes.empty(); }

private:
    using Clock = std::chrono::time_point<std::chrono::system_clock,
                                          std::chrono::nanoseconds>;

    std::shared_ptr<Clock>          episodeCheckpoint;
    double                          accumulatedEnvTime = 0.0;
    std::vector<uint16_t>           lastStepPos;
    std::vector<std::vector<uint16_t>> stepPositions;
    std::vector<int32_t>            currentEpisodeHeader;
    std::vector<std::vector<int32_t>> allEpisodes;
};

#endif // EPISODE_RECORDER_H
