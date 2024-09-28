#pragma once

#include <SweatCI.h>

#include <raylib.h>

namespace Meatball {
    namespace Config {
        struct OutputColors {
            static Color defaultColor;
            static Color echoColor;
            static Color warningColor;
            static Color errorColor;
        };
    }

    /// @note see Meatball::Config::OutputColors
    Color& outputLevelToOutputColor(const SweatCI::OutputLevel &level);
}