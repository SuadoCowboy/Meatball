#include "Config.h"

#include <filesystem>
#include <fstream>

#include <HayBCMD.h>

using Type = Meatball::ConfigData::Type;

Meatball::ConfigData::ConfigData(int i) : type(Type::INT), i(i) {}
Meatball::ConfigData::ConfigData(double d): type(Type::DOUBLE), d(d) {}
Meatball::ConfigData::ConfigData(float f): type(Type::FLOAT), f(f) {}
Meatball::ConfigData::ConfigData(bool b) : type(Type::BOOL), b(b) {}
Meatball::ConfigData::ConfigData(unsigned char uc) : type(Type::UNSIGNED_CHAR), uc(uc) {}
Meatball::ConfigData::ConfigData(std::string s): type(Type::STRING), s(s) {}
Meatball::ConfigData::ConfigData(Sound sound): type(Type::SOUND), sound(sound) {}
Meatball::ConfigData::ConfigData(Color color): type(Type::COLOR), color(color) {}

std::unordered_map<const char*, Meatball::ConfigData> Meatball::loadData(std::filesystem::path path) {
    if (!std::filesystem::exists(path) || std::filesystem::is_directory(path) || path.extension() != ".meatdata") {
        HayBCMD::Output::printf("ERROR: Could not load data: \"{}\" is not compatible or does not exist\n", path.c_str());
        return {};
    }

    std::ifstream file(path);

    int lineIdx = 0;
    std::string line;

    // line example: "mainPanelColor COLOR 22, 22, 22, 200"
    while (std::getline(file, line)) {
        size_t spaceIdx = line.find(" ");

        if (spaceIdx == std::string::npos) {
            HayBCMD::Output::printf("ERROR: Could not load data: space missing in line \"{}\"\n", lineIdx);
        }

        lineIdx++;
    }
}