#include "Json.h"

#include <fstream>
#include <filesystem>

bool Meatball::readJSONFile(const std::string& _path, json& outJObject) {
    {
        auto path = std::filesystem::path(_path);
        if (!std::filesystem::exists(path) || std::filesystem::is_directory(path))
            return false;
    }
    
    std::ifstream file(_path);

    if (!file) return false;

    std::string str;
    std::stringstream out;
    while (std::getline(file, str))
        out << str << ' ';

    outJObject = json::parse(out.str());

    return true;
}