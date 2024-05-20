#pragma once

#include <string>
#include <unordered_map>

#include <raylib.h>

namespace Meatball {
    struct ConfigData {
        enum Type { INT, DOUBLE, FLOAT, BOOL, UNSIGNED_CHAR, CHAR, STRING, SOUND, COLOR };
        Type type;
        
        int i;
        double d;
        float f;
        bool b;
        unsigned char uc;
        std::string s;
        
        Sound sound;
        Color color;

        ConfigData(int i);
        ConfigData(double d);
        ConfigData(float f);
        ConfigData(bool b);
        ConfigData(unsigned char uc);
        ConfigData(std::string s);
        ConfigData(Sound sound);
        ConfigData(Color color);
    };

    /// @brief A function that can be used to load data, not to be confused with loadCfg
    /// @param path path to a file that contains data (should end with .meatdata)
    /// @return empty unordered_map if something went wrong
    std::unordered_map<const char*, ConfigData> loadData(std::filesystem::path path);
}