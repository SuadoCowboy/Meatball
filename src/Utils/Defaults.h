#pragma once

#include <string>

#include <raylib.h>

#include "ConsoleUI.h"

namespace Meatball {
    /// @brief does the boring part for you
    /// @param rect position and size of the console
    /// @param meatdataPath the path to the .meatdata file
    /// console UI meatdata consists of:
    /// mainPanelColor COLOR RRR,GGG,BBB,AAA (AAA is optional)
    /// sendButtonText STRING EXAMPLE
    /// sendButtonColor COLOR RRR,GGG,BBB,AAA (AAA is optional)
    /// sendButtonHoveredColor COLOR RRR,GGG,BBB,AAA (AAA is optional)
    /// closeButtonColor COLOR RRR,GGG,BBB,AAA (AAA is optional)
    /// @return 
    ConsoleUIScene initConsoleUI(Rectangle rect, const std::string& meatdataPath);
}