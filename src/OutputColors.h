#pragma once

#include <SweatCI.h>

#include <raylib.h>

namespace Meatball {
    struct OutputColors {
        static Color defaultColor;
        static Color echoColor;
        static Color warningColor;
        static Color errorColor;
    };

    /// @note see Meatball::OutputColors
    Color& outputLevelToOutputColor(const SweatCI::OutputLevel &level);
}