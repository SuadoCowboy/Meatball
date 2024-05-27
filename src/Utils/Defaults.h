#pragma once

#include <string>

#include <raylib.h>

#include "ConsoleUI.h"

namespace Meatball {
    /// @brief does the boring part for you.
    /// @param rect position and size of the console
    /// @param meatdataPath the path to the .meatdata file
    /// @return ConsoleUIScene since it's not a static class
    /// @note console UI meatdata consists of:
    /// @note mainPanelColor COLOR RRR,GGG,BBB,AAA (AAA is optional for any variable that is a color)
    /// @note sendButtonColor COLOR RRR,GGG,BBB,AAA
    /// @note sendButtonHoveredColor COLOR RRR,GGG,BBB,AAA
    /// @note closeButtonColor COLOR RRR,GGG,BBB,AAA
    /// @note closeButtonHoveredColor COLOR RRR,GGG,BBB,AAA
    /// @note outputBoxColor COLOR RRR,GGG,BBB,AAA
    /// @note outputBoxTextColor COLOR RRR,GGG,BBB,AAA
    /// @note inputBoxColor COLOR RRR,GGG,BBB,AAA
    /// @note inputBoxTextColor COLOR RRR,GGG,BBB,AAA
    /// @note inputBoxCursorColor COLOR RRR,GGG,BBB,AAA
    /// @note fontSize UNSIGNED_CHAR 255 // [0-255]
    /// @note font STRING example/path/to/font/my font.otf
    /// @note autoCompleteTextColor COLOR RRR,GGG,BBB,AAA
    /// @note autoCompleteHightlightTextColor COLOR RRR,GGG,BBB,AAA
    /// @note autoCompleteSelectedTextColor COLOR RRR,GGG,BBB,AAA
    ConsoleUIScene initLocalConsole(Rectangle rect, const std::string& meatdataPath);
}