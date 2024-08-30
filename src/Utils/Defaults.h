#pragma once

#include <filesystem>
#include <string>

#include <raylib.h>

#include "../ConsoleUI.h"

namespace Meatball::Defaults {
    /// @brief loads all the needed data
    /// @param meatdataPath the path to the .meatdata file
    void init(const std::string& meatdataPath, Font& defaultFont);

    void loadConsoleFonts(ConsoleUI& consoleUI, const std::filesystem::path& fontPath, Font& outGeneralFont, Font& outLabelFont);

    /// @brief does the boring part for you.
    /// @param rect position and size of the console
    /// @param meatdataPath the path to the .meatdata file
    /// @return ConsoleUIScene since it's not a static class
    ConsoleUI initLocalConsole(const Rectangle& rect, const std::string& meatdataPath, Font& outGeneralFont, Font& outLabelFont);
}