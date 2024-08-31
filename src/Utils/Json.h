#pragma once

#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

#define GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, jObject, objectName, colorVariableName, colorVariable, jsonFilePath) \
    if (!hasKey || jObject.count(colorVariableName) == 0 \
    || !parseStringToColor(jObject[colorVariableName], colorVariable)) \
        HayBCMD::Output::printf(HayBCMD::WARNING, std::string("missing \"") + objectName + "." + colorVariableName + "\" on \"{}\" file\n", jsonFilePath);

#define GET_COLOR_FROM_JSON(jObject, colorVariableName, colorVariable, jsonFilePath) \
    if (jObject.count(colorVariableName) == 0 \
    || !parseStringToColor(jObject[colorVariableName], colorVariable)) \
        HayBCMD::Output::printf(HayBCMD::WARNING, std::string("missing \"") + colorVariableName + "\" on \"{}\" file\n", jsonFilePath);

#define GET_STRING_FROM_JSON(jObject, stringVariableName, stringVariable, jsonFilePath) \
    if (jObject.count(stringVariableName) == 0 || !jObject[stringVariableName].is_string()) \
        HayBCMD::Output::printf(HayBCMD::WARNING, std::string("missing \"") + stringVariableName + "\" on \"{}\" file\n", jsonFilePath); \
    else \
        stringVariable = jObject[stringVariableName];

namespace Meatball {
    /// @brief parses JSON File into a json file
    /// @return false if is not a directory or does not exists or if could not read file
    bool readJSONFile(const std::string& path, json& outJObject);
}