#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>

#include <raylib.h>

namespace Meatball::Config {
    enum ConfigType {
        STRING = 0,
        INT,
        FLOAT,
        DOUBLE,
        BOOL,
        UNSIGNED_CHAR,
        COLOR,
    };
    
    struct ConfigData {
        virtual ~ConfigData() {}

        ConfigType type;
    };

    template<typename T>
    struct ConfigTypeData : public ConfigData {
        T value;

        ConfigTypeData() = default;
        ConfigTypeData(const T& value) : value(value) {}
    };

    void clearData(std::unordered_map<std::string, Meatball::Config::ConfigData*>& map);
    
    /// @return nullptr if is not that type
    template<typename T>
    inline Config::ConfigTypeData<T>* getConfig(Config::ConfigData* data) {
        return dynamic_cast<Config::ConfigTypeData<T>*>(data);
    }

    /// @brief if data contains 
    /// @param data loaded data (see loadData function)
    /// @param what what to check if is contained in data
    /// @return pointer to data if contains else nullptr
    ConfigData* ifContainsGet(std::unordered_map<std::string, ConfigData*>& data, const std::string& what);

    /// @brief A function that can be used to load meatdata
    /// @param path path to a file that contains meatdata (should end with .meatdata)
    /// @return data unless if something went wrong, then returns a empty unordered_map
    /// @warning do not forget to use clearData function to delete data afterwards
    std::unordered_map<std::string, ConfigData*> loadData(const std::filesystem::path& path);

    /// @brief saves data as a meatdata file
    bool saveData(const std::filesystem::path& path, std::unordered_map<std::string, ConfigData*>& dataMap);
}