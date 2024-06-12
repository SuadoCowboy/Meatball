#pragma once

#include <filesystem>
#include <string>

#include <raylib.h>

#include "ConsoleUI.h"

namespace Meatball { namespace Defaults {
    /// @brief loads all the needed data
    /// @param meatdataPath the path to the .meatdata file
    void init(const std::string& meatdataPath);

    void loadConsoleFonts(ConsoleUIScene& consoleUI, const std::filesystem::path& fontPath);

    /// @brief does the boring part for you.
    /// @param rect position and size of the console
    /// @param meatdataPath the path to the .meatdata file
    /// @return ConsoleUIScene since it's not a static class
    ConsoleUIScene initLocalConsole(const Rectangle& rect, const std::string &meatdataPath);
}}