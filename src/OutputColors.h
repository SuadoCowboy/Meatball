#pragma once

#include <memory>

#include <HayBCMD.h>

#include <raylib.h>

namespace Meatball {
    namespace Config {
        struct OutputColors {
            static std::shared_ptr<Color> defaultColor;
            static std::shared_ptr<Color> echoColor;
            static std::shared_ptr<Color> warningColor;
            static std::shared_ptr<Color> errorColor;
        };
    }

    /// @note see Meatball::Config::OutputColors
    const std::shared_ptr<Color>& outputLevelToOutputColor(const HayBCMD::OutputLevel &level);
}