#include "Config.h"

#include <fstream>

#include <HayBCMD.h>

Meatball::Config::ConfigData::ConfigData(int i) : intV(i) {}
Meatball::Config::ConfigData::ConfigData(double d): doubleV(d) {}
Meatball::Config::ConfigData::ConfigData(float f): floatV(f) {}
Meatball::Config::ConfigData::ConfigData(bool b) : boolV(b) {}
Meatball::Config::ConfigData::ConfigData(unsigned char uc) : unsignedCharV(uc) {}
Meatball::Config::ConfigData::ConfigData(std::string s): stringV(s) {}
Meatball::Config::ConfigData::ConfigData(Color color): colorV(color) {}
Meatball::Config::ConfigData::ConfigData() {}

static bool handleSpaceError(size_t spaceIdx, size_t spaceIdxBefore, size_t lineIdx) {
    if (spaceIdx == std::string::npos || spaceIdx == 0 || spaceIdx == spaceIdxBefore+1) {
            HayBCMD::Output::printf("ERROR: Could not load data: space error in line {}\n", lineIdx);
            return true;
    }

    return false;
}

Meatball::Config::ConfigData* Meatball::Config::ifContainsGet(std::unordered_map<std::string, ConfigData>& data, const std::string& what) {
    return data.count(what)? &data[what] : nullptr;
}

std::unordered_map<std::string, Meatball::Config::ConfigData> Meatball::Config::loadData(std::filesystem::path path) {
    if (!std::filesystem::exists(path) || std::filesystem::is_directory(path) || path.extension() != ".meatdata") {
        HayBCMD::Output::printf("ERROR: Could not load data: \"{}\" is not compatible or does not exist\n", path.c_str());
        return {};
    }

    std::ifstream file(path);

    std::unordered_map<std::string, Config::ConfigData> data = {};

    // line example: "mainPanelColor COLOR 22,22,22,200"
    size_t lineIdx = 1;
    for (std::string line; std::getline(file, line);) {
        if (line.size() <= 1) continue;
        
        // "name "
        size_t spaceIdx = line.find(" ");
        if (handleSpaceError(spaceIdx, 0, lineIdx)) return {};
        
        std::string name = line.substr(0, spaceIdx);

        // " type "
        size_t secondSpaceIdx = line.find(" ", spaceIdx+1);
        if (handleSpaceError(secondSpaceIdx, spaceIdx, lineIdx)) return {};

        std::string type = line.substr(spaceIdx+1, secondSpaceIdx-spaceIdx-1);
        std::string value = line.substr(secondSpaceIdx+1);

        if (type == "STRING")
            data[name.c_str()] = ConfigData(value);
        else if (type == "INT")
            data[name.c_str()] = ConfigData(std::stoi(value));
        else if (type == "FLOAT")
            data[name.c_str()] = ConfigData(std::stof(value));
        else if (type == "DOUBLE")
            data[name.c_str()] = ConfigData(std::stod(value));
        else if (type == "BOOL")
            data[name.c_str()] = ConfigData((bool)std::stoi(value));
        else if (type == "UNSIGNED_CHAR")
            data[name.c_str()] = ConfigData((unsigned char)std::stoi(value));
        
        else if (type == "COLOR") {
            size_t firstCommaIdx = value.find(",");
            size_t secondCommaIdx = value.find(",", firstCommaIdx+1);
            size_t thirdCommaIdx = value.find(",", secondCommaIdx+1);

            Color color = BLACK;
            
            color.r = std::stoi(value.substr(0, firstCommaIdx));
            color.g = std::stoi(value.substr(firstCommaIdx+1, secondCommaIdx-firstCommaIdx-1));
            color.b = std::stoi(value.substr(secondCommaIdx+1, thirdCommaIdx-secondCommaIdx-1));
            if (thirdCommaIdx != std::string::npos)
                color.a = std::stoi(value.substr(thirdCommaIdx+1));

            data[name.c_str()] = ConfigData(color);
        }

        else {
            HayBCMD::Output::printf("ERROR: Could not load data: missing TYPE in line {}\n", lineIdx);
            return {};
        }
        
        lineIdx++;
    }

    return data;
}