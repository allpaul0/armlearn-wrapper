#include <fstream>
#include <iostream>
#include <json.h>

#include "codegenParameters.h"

void CodeGenParameters::readConfigFile(const char* path, Json::Value& root)
{
    std::ifstream ifs;
    ifs.open(path);

    if (!ifs.is_open()) {
        std::cerr << "Error : specified param file doesn't exist : " << path << std::endl;
        std::cerr << "\033[1;31mmake sure you are not executing from a different directory than the one containing the params folder.\033[0m" << std::endl;
        throw Json::Exception("aborting");
    }

    Json::CharReaderBuilder builder;
    builder["collectComments"] = true;
    JSONCPP_STRING errs;
    if (!parseFromStream(builder, ifs, &root, &errs)) {
        std::cout << errs << std::endl;
        std::cerr << "Ignoring ill-formed config file " << path << std::endl;
    }
}

void CodeGenParameters::setAllParamsFrom(const Json::Value& root)
{
    for (std::string const& key : root.getMemberNames()) {
        if (root[key].size() == 0) {
            // we have a parameter without subtree (as a leaf)
            Json::Value value = root[key];
            setParameterFromString(key, value);
        }
    }
}

void CodeGenParameters::setParameterFromString(const std::string& param, Json::Value const& value)
{
    if (param == "isInstrumented"){
        isInstrumented = (bool)value.asBool();
        return; 
    }
    if (param == "isDecorated") {
        isDecorated = (bool)value.asBool();
        return;
    }

    // we didn't recognize the symbol
    std::cerr << "Ignoring unknown parameter " << param << std::endl;
}

void CodeGenParameters::loadParametersFromJson(const char* path)
{
    Json::Value root;
    readConfigFile(path, root);
    setAllParamsFrom(root);
}
