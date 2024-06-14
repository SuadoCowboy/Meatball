#include "Config.h"

#include <fstream>

#include "Console.h"


static bool handleSpaceError(size_t spaceIdx, size_t spaceIdxBefore, size_t lineIdx) {
    if (spaceIdx == std::string::npos || spaceIdx == 0 || spaceIdx == spaceIdxBefore+1) {
        Meatball::Console::printf(HayBCMD::OutputLevel::ERROR, "could not load data: space error in line {}", lineIdx);
        return true;
    }

    return false;
}

void Meatball::Config::clearData(std::unordered_map<std::string, Meatball::Config::ConfigData*>& map) {
    for (auto& data : map)
        delete data.second;

    map.clear();
}

Meatball::Config::ConfigData* Meatball::Config::ifContainsGet(std::unordered_map<std::string, Meatball::Config::ConfigData*>& data, const std::string& what) {
    return data.count(what)? data[what] : nullptr;
}

std::unordered_map<std::string, Meatball::Config::ConfigData*> Meatball::Config::loadData(std::filesystem::path path) {
    if (!std::filesystem::exists(path) || std::filesystem::is_directory(path) || path.extension() != ".meatdata") {
        Console::printf(HayBCMD::OutputLevel::ERROR, "could not load data: \"{}\" is not compatible or does not exist\n", path.string());
        return {};
    }

    std::ifstream file(path);

    std::unordered_map<std::string, Config::ConfigData*> data{};

    // line example: "mainPanelColor COLOR 22,22,22,200"
    size_t lineIdx = 1;
    for (std::string line; std::getline(file, line);) {
        if (line.size() <= 1) continue;
        
        // "name "
        size_t spaceIdx = line.find(" ");
        if (handleSpaceError(spaceIdx, 0, lineIdx)) {
            clearData(data);
            return data;
        }
        
        std::string name = line.substr(0, spaceIdx);

        // " type "
        size_t secondSpaceIdx = line.find(" ", spaceIdx+1);
        if (handleSpaceError(secondSpaceIdx, spaceIdx, lineIdx)) {
            clearData(data);
            return data;
        }

        std::string type = line.substr(spaceIdx+1, secondSpaceIdx-spaceIdx-1);
        std::string value = line.substr(secondSpaceIdx+1);

        if (type == "STRING") {
            data[name] = new Config::ConfigTypeData(value);
            data[name]->type = STRING;
        } else if (type == "INT") {
            data[name] = new Config::ConfigTypeData(value);
            data[name]->type = INT;
        } else if (type == "FLOAT") {
            data[name] = new Config::ConfigTypeData(value);
            data[name]->type = FLOAT;
        } else if (type == "DOUBLE") {
            data[name] = new Config::ConfigTypeData(value);
            data[name]->type = DOUBLE;
        } else if (type == "BOOL") {
            data[name] = new Config::ConfigTypeData(value);
            data[name]->type = BOOL;
        } else if (type == "UNSIGNED_CHAR") {
            data[name] = new Config::ConfigTypeData(value);
            data[name]->type = UNSIGNED_CHAR;
        }
        
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

            data[name] = new Config::ConfigTypeData(color);
            data[name]->type = COLOR;
        }

        else {
            Meatball::Console::printf(HayBCMD::OutputLevel::ERROR, "could not load data: missing TYPE in line {}\n", lineIdx);
            clearData(data);
            return data;
        }
        
        lineIdx++;
    }

    return data;
}