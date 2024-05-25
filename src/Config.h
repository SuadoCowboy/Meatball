#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>

#include <raylib.h>

namespace Meatball {
    namespace Config {
        struct ConfigData {
            int intV;
            double doubleV;
            float floatV;
            bool boolV;
            unsigned char unsignedCharV;
            std::string stringV;
            
            Color colorV;

            ConfigData(int i);
            ConfigData(double d);
            ConfigData(float f);
            ConfigData(bool b);
            ConfigData(unsigned char uc);
            ConfigData(std::string s);
            ConfigData(Color color);
            ConfigData();
        };

        /// @brief if data contains 
        /// @param data loaded data (see loadData function)
        /// @param what what to check if is contained in data
        /// @return pointer to data if contains else nullptr
        ConfigData* ifContainsGet(std::unordered_map<std::string, ConfigData>& data, const std::string& what);

        /// @brief A function that can be used to load data, not to be confused with loadCfg
        /// @param path path to a file that contains data (should end with .meatdata)
        /// @return data. If something went wrong it returns the data that got before the error
        std::unordered_map<std::string, ConfigData> loadData(std::filesystem::path path);
    }
}