#include "Config.h"

#include <fstream>

#include "Console.h"
#include "Utils/Utils.h"

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

std::unordered_map<std::string, Meatball::Config::ConfigData*> Meatball::Config::loadData(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path) || std::filesystem::is_directory(path) || path.extension().string() != ".meatdata") {
        Console::printf(HayBCMD::ERROR, "could not load data: \"{}\" is not compatible or does not exist\n", path.string());
        return {};
    }

    std::ifstream file(path);

    std::unordered_map<std::string, ConfigData*> data{};

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
            data[name] = new ConfigTypeData(value);
            data[name]->type = STRING;
        } else if (type == "INT") {
            data[name] = new ConfigTypeData(std::stoi(value));
            data[name]->type = INT;
        } else if (type == "FLOAT") {
            data[name] = new ConfigTypeData(std::stof(value));
            data[name]->type = FLOAT;
        } else if (type == "DOUBLE") {
            data[name] = new ConfigTypeData(std::stod(value));
            data[name]->type = DOUBLE;
        } else if (type == "BOOL") {
            data[name] = new ConfigTypeData((bool)std::stoi(value));
            data[name]->type = BOOL;
        } else if (type == "UNSIGNED_CHAR") {
            data[name] = new ConfigTypeData((unsigned char)std::stoi(value));
            data[name]->type = UNSIGNED_CHAR;
        } else if (type == "COLOR") {
            Color buf = BLACK;
            parseStringToColor(value, buf);
            data[name] = new ConfigTypeData(buf);
            data[name]->type = COLOR;
        }

        else {
            Console::printf(HayBCMD::ERROR, "could not load data: missing TYPE in line {}\n", lineIdx);
            clearData(data);
            return data;
        }
        
        lineIdx++;
    }

    return data;
}

bool Meatball::Config::saveData(const std::filesystem::path& path, std::unordered_map<std::string, ConfigData*>& dataMap) {
    if (std::filesystem::is_directory(path)) {
        Console::printf(HayBCMD::ERROR, "could not save data because path \"{}\" is a directory", path.string());
        return false;
    }

    std::ofstream file(path);

    if (!file) {
        Console::printf(HayBCMD::ERROR, "could not open file \"{}\"", path.string());
        return false;
    }

    std::stringstream out;
    for (auto& data : dataMap) {
        out << data.first << " ";
        switch (data.second->type) {
        case ConfigType::BOOL:
            out << "BOOL " << getConfig<bool>(data.second)->value;
            break;
        case ConfigType::DOUBLE:
            out << "DOUBLE " << getConfig<double>(data.second)->value;
            break;
        case ConfigType::FLOAT:
            out << "FLOAT " << getConfig<float>(data.second)->value;
            break;
        case ConfigType::INT:
            out << "INT " << getConfig<int>(data.second)->value;
            break;
        case ConfigType::STRING:
            out << "STRING " << getConfig<std::string>(data.second)->value;
            break;
        case ConfigType::UNSIGNED_CHAR:
            out << "UNSIGNED_CHAR " << getConfig<unsigned char>(data.second)->value;
            break;
        case ConfigType::COLOR:
            Color color = getConfig<Color>(data.second)->value;
            out << "COLOR "
                << color.r << ","
                << color.g << ","
                << color.b;
            if (color.a != 255)
                out << "," << color.a;
            break;
        }

        out << "\n";
    }

    file << out.str();

    return true;
}