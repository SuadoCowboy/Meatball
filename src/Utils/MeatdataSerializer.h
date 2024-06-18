#pragma once

#include <fstream>
#include <unordered_map>
#include <string>

#include "Config.h"

namespace Meatball {
    void serializeString(std::ofstream& out, const std::string& str);
    std::string deserializeString(std::ifstream& in); 

    void serializeConfigData(std::ofstream& out, Config::ConfigData* data);
    Config::ConfigData* deserializeConfigData(std::ifstream& in);

    bool serializeConfigDataMapToFile(const std::unordered_map<std::string, Config::ConfigData*>& map, const std::string& filename);
    std::unordered_map<std::string, Config::ConfigData*> deserializeConfigDataMapFromFile(const std::string& filename);
}