#pragma once

#include <string>

#include <raylib.h>

#include "ConsoleUI.h"

namespace Meatball {
    /// @brief does the boring part for you.
    /// @param rect position and size of the console
    /// @param meatdataPath the path to the .meatdata file
    /// @note console UI meatdata consists of:
    /// @note mainPanelColor COLOR RRR,GGG,BBB,AAA (AAA is optional)
    /// @note sendButtonText STRING EXAMPLE
    /// @note sendButtonColor COLOR RRR,GGG,BBB,AAA (AAA is optional)
    /// @note sendButtonHoveredColor COLOR RRR,GGG,BBB,AAA (AAA is optional)
    /// @note closeButtonColor COLOR RRR,GGG,BBB,AAA (AAA is optional)
    /// @return ConsoleUIScene since it's not a static class
    ConsoleUIScene initLocalConsole(Rectangle rect, const std::string& meatdataPath);
}