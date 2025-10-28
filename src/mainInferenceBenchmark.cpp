#include <iostream>
#include <filesystem>
#include <chrono>
#include <vector>
#include <cmath>
#include <json.h>
#include <fstream>

#include "codegen/externHeader.h"
#include "precalcul/seeds_nbActionsToTerminal.h"
#include "codegen/codeGenArmlearn.h"

#define NB_ACTIONS_INF 1E2

// Local static buffers that will hold the values the generated code reads.
// Their lifetime is static so the pointers we give to the generated engine remain valid.
static typeInf in1_buf[3];
static typeInf in2_buf[3];
static typeInf in3_buf[3];
static typeInf in4_buf[6];

/* Fill the buffers with values for the given seed and point the global pointers
   at those buffers so generated code can use in1[0], in1[1], ... */
void assign_LE_values(int seed)
{
    // copy values into contiguous buffers (convert if needed)
    in1_buf[0] = dataSourcesLE_0[seed];
    in1_buf[1] = dataSourcesLE_1[seed];
    in1_buf[2] = dataSourcesLE_2[seed];

    in2_buf[0] = dataSourcesLE_3[seed];
    in2_buf[1] = dataSourcesLE_4[seed];
    in2_buf[2] = dataSourcesLE_5[seed];

    in3_buf[0] = dataSourcesLE_6[seed];
    in3_buf[1] = dataSourcesLE_7[seed];
    in3_buf[2] = dataSourcesLE_8[seed];

    in4_buf[0] = dataSourcesLE_9[seed];
    in4_buf[1] = dataSourcesLE_10[seed];
    in4_buf[2] = dataSourcesLE_11[seed];
    in4_buf[3] = dataSourcesLE_12[seed];
    in4_buf[4] = dataSourcesLE_13[seed];
    in4_buf[5] = dataSourcesLE_14[seed];

    // point the global extern pointers used by generated code at our buffers
    in1 = in1_buf;
    in2 = in2_buf;
    in3 = in3_buf;
    in4 = in4_buf;
}

int main()
{
    typeInf actionID = -1;
    std::chrono::_V2::system_clock::time_point start, end;

    // to store latencies per class
    std::vector<double> latency[NB_CLASSES];

    std::cout << "Start inference benchmark" << std::endl;

    for (int seed = 0; seed < NB_SEED; seed++)
    {
        assign_LE_values(seed);

        start = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < NB_ACTIONS_INF; j++)
        {
            inferenceTPG(&actionID);
        }
        end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> cpu_time_used = end - start;

        double time_ns = cpu_time_used.count() / NB_ACTIONS_INF * 1e9;

        // the class of this traversal is ids_graph_traversals[seed]
        latency[ids_graph_traversals[seed]].push_back(time_ns);
    }
    std::cout << "End inference benchmark" << std::endl;

    // JSON output root
    Json::Value root;
    Json::Value results(Json::arrayValue);

    // Compute and print averages + stddevs per class
    for (int c = 0; c < NB_CLASSES; c++)
    {
        if (latency[c].empty())
            continue;

        double sum = 0.0;
        for (double t : latency[c])
        {
            sum += t;
        }
        double mean = sum / latency[c].size();

        double sq_sum = 0.0;
        for (double t : latency[c])
        {
            sq_sum += (t - mean) * (t - mean);
        }
        double stddev = std::sqrt(sq_sum / latency[c].size());

        std::cout << "Class " << c
                  << ": Average latency = " << mean
                  << " ns, Stddev = " << stddev << " ns"
                  << std::endl;

        // Add to JSON table
        Json::Value entry;
        entry["Class"] = c;
        entry["AvgLatency_ns"] = mean;
        entry["Stddev_ns"] = stddev;
        results.append(entry);
    }

    root["results"] = results;

    // Write results to file
    std::ofstream file("outLogs/latency_results.json");
    if (file.is_open())
    {
        Json::StreamWriterBuilder writer;
        writer["indentation"] = "  "; // pretty-print
        file << Json::writeString(writer, root);
        file.close();
        std::cout << "Results written to latency_results.json" << std::endl;
    }
    else
    {
        std::cerr << "Failed to open latency_results.json for writing!" << std::endl;
    }

    return 0;
}