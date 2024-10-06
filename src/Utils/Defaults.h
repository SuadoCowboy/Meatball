#pragma once

#include <filesystem>
#include <string>

#include <raylib.h>

#include "../ConsoleUI.h"

namespace Meatball::Defaults {
    /// @return false if one of the loading fonts went wrong(but still tries to load the rest)
    bool loadConsoleFonts(ConsoleUI& consoleUI, const std::filesystem::path& fontPath, Font& outInputFont, Font& outOutputFont, Font& labelFont);

    /// @brief gets all the needed console ui data and initializes
    /// @param rect position and size of the console
    ConsoleUI initLocalConsole(const Rectangle& rect, const std::string& jsonPath, Font& outInputFont, Font& outOutputFont, Font& outLabelFont);
}