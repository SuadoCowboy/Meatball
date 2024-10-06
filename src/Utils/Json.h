#pragma once

#include <string>

#include <raylib.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Meatball {
    /// @brief parses JSON File into a nlohmann::json object
    /// @return false if is not a directory, does not exists or if could not read file
    bool parseJsonFile(const std::string& path, json& output);

    class Json {
    public:
        json object;
        std::string filePath = "", objectName = "";

        Json() {}

        Json(const json& object, const std::string& filePath, const std::string& objectName="")
         : object(object), filePath(filePath), objectName(objectName) {};
        
        Json(const Json& other, const std::string& key);
        Json(const std::string& filePath);

        bool getJson(const std::string& key, Meatball::Json& json) const;

        bool getString(const std::string& key, std::string& string) const;
        bool getColor(const std::string& key, Color& color) const;
        
        bool getInt(const std::string& key, int& number) const;
        bool getFloat(const std::string& key, float& number) const;
        bool getUnsigned(const std::string& key, unsigned int& number) const;
    };
}