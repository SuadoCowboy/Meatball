#include "MeatdataSerializer.h"

#include <HayBCMD.h>

#include "Console.h"

void Meatball::serializeString(std::ofstream& out, const std::string& str) {
    unsigned char length = str.length();
    out.write(reinterpret_cast<const char*>(&length), sizeof(length));
    out.write(str.c_str(), length);
}

std::string Meatball::deserializeString(std::ifstream& in) {
    unsigned char length;
    in.read(reinterpret_cast<char*>(&length), sizeof(length));
    std::string str(length, ' ');
    in.read(&str[0], length);
    return str;
}

void Meatball::serializeConfigData(std::ofstream& out, Meatball::Config::ConfigData* data) {
    out.write(reinterpret_cast<const char*>(&data->type), sizeof(data->type));

    if (data->type == Config::STRING) {
        auto value = Config::getConfig<std::string>(data)->value;
        serializeString(out, value);
    }
    else if (data->type == Config::INT) {
        auto value = Config::getConfig<int>(data)->value;
        out.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }
    else if (data->type == Config::FLOAT) {
        auto value = Config::getConfig<float>(data)->value;
        out.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }
    else if (data->type == Config::DOUBLE) {
        auto value = Config::getConfig<double>(data)->value;
        out.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }
    else if (data->type == Config::BOOL) {
        auto value = Config::getConfig<bool>(data)->value;
        out.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }
    else if (data->type == Config::UNSIGNED_CHAR) {
        auto value = Config::getConfig<unsigned char>(data)->value;
        out.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }
    else if (data->type == Config::COLOR) {
        auto value = Config::getConfig<Color>(data)->value;
        out.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }
}

Meatball::Config::ConfigData* Meatball::deserializeConfigData(std::ifstream& in) {
    Config::ConfigType type;
    in.read(reinterpret_cast<char*>(&type), sizeof(type));
    
    if (type == Config::STRING) {
        auto data = new Config::ConfigTypeData<std::string>(deserializeString(in));
        data->type = Config::STRING;
        return data;
    }
    else if (type == Config::INT) {
        int value;
        in.read(reinterpret_cast<char*>(&value), sizeof(value));
        auto data = new Config::ConfigTypeData<int>(value);
        data->type = Config::INT;
        return data;
    }
    else if (type == Config::FLOAT) {
        float value;
        in.read(reinterpret_cast<char*>(&value), sizeof(value));
        auto data = new Config::ConfigTypeData<float>(value);
        data->type = Config::FLOAT;
        return data;
    }
    else if (type == Config::DOUBLE) {
        double value;
        in.read(reinterpret_cast<char*>(&value), sizeof(value));
        auto data = new Config::ConfigTypeData<double>(value);
        data->type = Config::DOUBLE;
        return data;
    }
    else if (type == Config::UNSIGNED_CHAR) {
        unsigned char value;
        in.read(reinterpret_cast<char*>(&value), sizeof(value));
        auto data = new Config::ConfigTypeData<unsigned char>(value);
        data->type = Config::UNSIGNED_CHAR;
        return data;
    }
    else if (type == Config::BOOL) {
        bool value;
        in.read(reinterpret_cast<char*>(&value), sizeof(value));
        auto data = new Config::ConfigTypeData<bool>(value);
        data->type = Config::BOOL;
        return data;
    }
    else if (type == Config::COLOR) {
        Color value;
        in.read(reinterpret_cast<char*>(&value), sizeof(value));
        auto data = new Config::ConfigTypeData<Color>(value);
        data->type = Config::COLOR;
        return data;
    }

    return nullptr;
}

bool Meatball::serializeConfigDataMapToFile(const std::unordered_map<std::string, Meatball::Config::ConfigData*>& map, const std::string& filename) {
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) {
        Console::print(HayBCMD::ERROR, "failed to open file for writing\n");
        return false;
    }

    {
        unsigned short mapSize = map.size();
        out.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));
    }

    for (const auto& pair : map) {
        serializeString(out, pair.first);
        serializeConfigData(out, pair.second);
    }

    out.close();

    return true;
}

std::unordered_map<std::string, Meatball::Config::ConfigData*> Meatball::deserializeConfigDataMapFromFile(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        Console::print(HayBCMD::ERROR, "failed to open file for reading\n");
        return {};
    }

    unsigned short mapSize;
    in.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));

    std::unordered_map<std::string, Config::ConfigData*> map;
    for (size_t i = 0; i < mapSize; ++i) {
        std::string key = deserializeString(in);
        map[key] = deserializeConfigData(in);
        
        if (map[key] == nullptr)
            for (auto& pair : map) {
                delete pair.second;
                if (pair.second == nullptr) return {};
            }
    }

    in.close();
    return map;
}