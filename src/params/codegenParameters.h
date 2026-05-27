
#ifndef CODEGEN_PARAMETERS_H
#define CODEGEN_PARAMETERS_H

#include <thread>
#include <iostream>

namespace Json {
    class Value;
}

class CodeGenParameters {


private:
    /**
     * \brief Puts the parameters described in the derivative tree root in
     * given CodeGenParameters.
     *
     * Browses the JSON tree. If the node we're looking at is a leaf,
     * we call setParameterFromString. Otherwise, we browe its children to
     * follow the known parameters structure.
     *
     * \param[in] root JSON tree we will use to set parameters.
     */
    void setAllParamsFrom(const Json::Value& root);

    /**
     * \brief Reads a given json file and puts the derivative tree in root.
     *
     * Opens the file and calls the parseFromStream() method from JsonCpp
     * which handles all the parsing of the JSON file. It eventually returns
     * errors in a parameter, e.g. if the file does not respect JSON format.
     * In this case, the file is simply ignored and it is logged explicitly.
     * However, in case of JsonCpp internal errors, there can be exceptions,
     * as described in throws.
     *
     * \param[in] path path of the JSON file from which the parameters are
     *            read.
     * \param[out] root JSON tree we are going to build with the file.
     * \throws std::exception if json parser settings are not in their
     * right formats.
     */
    void readConfigFile(const char* path, Json::Value& root);

    /**
     * \brief Given a parameter name, sets its value in given
     * CodeGenParameters object.
     *
     * To find the right parameter, the method contains a lot of if
     * statements, each of them finishing by a return. These statements
     * compare the given parameter name to known parameters names.
     * If a parameter is found, it casts value to the right type and sets
     * the given parameter to this value.
     * If no parameter was found, it simply ignores the input and logs it
     * explicitly.
     *
     * \param[in] param the name of the LearningParameters being updated.
     * \param[in] value the value we want to set the parameter to.
     */
    void setParameterFromString(const std::string& param,
                                Json::Value const& value);

public:

    // Instrumentation at Team level (start, compute all progs, end) 
    // used for Basic Block analysis of TPG latency
    bool isInstrumented = false;

    // Decoration means adding labels before Team sections to ease 
    // parsing of assembly code 
    bool isDecorated = false;

    /**
     * \brief Loads configuration from a JSON file and updates 
     * this object's parameters.
     *
     * High level method that simply calls more complicated ones as follow :
     * - readConfigFile to get the derivative tree from a JSON file path
     * - setAllParamsFrom to set the parameters given the obtained tree.
     *
     * \param[in] path path of the JSON file from which the parameters are
     */
    void loadParametersFromJson(const char* path);

};

#endif
