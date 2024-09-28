#include "Json.h"

#include <fstream>
#include <filesystem>
#include <sstream>

#include <SweatCI.h>

#include "Utils/Utils.h"

static void prepareWarn(std::stringstream& oss, const std::string& key, const std::string& filePath, const std::string& objectName) {
    oss << " \"";

    if (!objectName.empty())
        oss << objectName << ".";

    oss << key << "\"";

    if (!filePath.empty())
        oss << " on \"" << filePath << "\" file\n";
    else
        oss << "\n";
}

static void warnMissingKey(const std::string& key, const std::string& filePath, const std::string& objectName) {
    std::stringstream oss;
    oss << "missing";

    prepareWarn(oss, key, filePath, objectName);

    SweatCI::Output::print(SweatCI::WARNING, oss.str());
}

static void warnWrongType(const std::string& key, const std::string& typeName, const std::string& filePath, const std::string& objectName) {
    std::stringstream oss;
    oss << "wrong type. Expected \"" << typeName << "\" for";

    prepareWarn(oss, key, filePath, objectName);

    SweatCI::Output::print(SweatCI::WARNING, oss.str());
}

Meatball::Json::Json(const Json& other, const std::string& key) {
    other.getJson(key, *this);
}

Meatball::Json::Json(const std::string& filePath) : filePath(filePath) {
    if (!parseJsonFile(filePath, object))
        SweatCI::Output::printf(SweatCI::ERROR, "could not read \"{}\" file\n", filePath);
}

bool Meatball::Json::getJson(const std::string& key, Meatball::Json& json) const {
    if (object.count(key) == 0) {
        warnMissingKey(key, filePath, objectName);
        return false;
    } else if (!object[key].is_object()) {
        warnWrongType(key, "object", filePath, objectName);
        return false;
    }

    json.object = object[key];
    json.objectName = key;
    json.filePath = filePath;

    if (!objectName.empty())
        json.objectName = objectName + "." + json.objectName;

    return true;
}

bool Meatball::Json::getString(const std::string& key, std::string& string) const {
    if (object.count(key) == 0) {
        warnMissingKey(key, filePath, objectName);
        return false;
    } else if (!object[key].is_string()) {
        warnWrongType(key, "string", filePath, objectName);
        return false;
    }

    string = object[key];
    return true;
}

bool Meatball::Json::getColor(const std::string& key, Color& color) const {
    if (object.count(key) == 0) {
        warnMissingKey(key, filePath, objectName);
        return false;
    } else if (!parseStringToColor(object[key], color)) {
        warnWrongType(key, "color", filePath, objectName);
        return false;
    }

    return true;
}

bool Meatball::Json::getInt(const std::string& key, int& number) const {
    if (object.count(key) == 0) {
        warnMissingKey(key, filePath, objectName);
        return false;
    } else if (!object[key].is_number_integer()) {
        warnWrongType(key, "integer", filePath, objectName);
        return false;
    }

    number = object[key];

    return true;
}

bool Meatball::Json::getFloat(const std::string& key, float& number) const {
    if (object.count(key) == 0) {
        warnMissingKey(key, filePath, objectName);
        return false;
    } else if (!object.is_number_float()) {
        warnWrongType(key, "float", filePath, objectName);
        return false;
    }

    number = object[key];

    return true;
}

bool Meatball::Json::getUnsigned(const std::string& key, unsigned int& number) const {
    if (object.count(key) == 0) {
        warnMissingKey(key, filePath, objectName);
        return false;
    } else if (!object.is_number_unsigned()) {
        warnWrongType(key, "unsigned integer", filePath, objectName);
        return false;
    }

    number = object[key];

    return true;
}

bool Meatball::parseJsonFile(const std::string& _path, json& output) {
    {
        std::filesystem::path path{_path};
        if (!std::filesystem::exists(path) || std::filesystem::is_directory(path))
            return false;
    }
    
    std::ifstream file(_path);

    if (!file) return false;

    std::string str;
    std::stringstream out;
    while (std::getline(file, str))
        out << str << ' ';

    output = json::parse(out.str());

    return true;
}