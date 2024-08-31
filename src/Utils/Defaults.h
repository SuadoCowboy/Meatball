#pragma once

#include <filesystem>
#include <string>

#include <raylib.h>

#include "../ConsoleUI.h"

namespace Meatball::Defaults {
    /// @brief loads all the needed data
    void init(const std::string& jsonPath, Font& defaultFont);

    void loadConsoleFonts(ConsoleUI& consoleUI, const std::filesystem::path& fontPath, Font& outGeneralFont, Font& labelFont);

    /// @brief does the boring part for you.
    /// @param rect position and size of the console
    ConsoleUI initLocalConsole(const Rectangle& rect, const std::string& jsonPath, Font& outGeneralFont, Font& outLabelFont);
}