#pragma once

#include <filesystem>
#include <string>

#include <raylib.h>

#include "../ConsoleUI.h"

namespace Meatball::Defaults {
    /// @brief loads all the needed data
    /// @param defaultFont if the json file contains a valid defaultFont path, this will change or else it will be whatever was set before
    void init(const std::string& jsonPath, Font& defaultFont);

    /// @return false if one of the loading fonts went wrong(but still tries to load the rest)
    bool loadConsoleFonts(ConsoleUI& consoleUI, const std::filesystem::path& fontPath, Font& outGeneralFont, Font& labelFont);

    /// @brief does the boring part for you.
    /// @param rect position and size of the console
    ConsoleUI initLocalConsole(const Rectangle& rect, const std::string& jsonPath, Font& outGeneralFont, Font& outLabelFont);
}