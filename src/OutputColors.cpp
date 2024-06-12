#include "OutputColors.h"

Color Meatball::Config::OutputColors::defaultColor = WHITE;
// Light blue
Color Meatball::Config::OutputColors::echoColor = {173, 216, 230, 255};
// Light yellow
Color Meatball::Config::OutputColors::warningColor = {253, 250, 114, 255};
Color Meatball::Config::OutputColors::errorColor = RED;

Color& Meatball::outputLevelToOutputColor(const HayBCMD::OutputLevel& level) {
    switch (level) {
    case HayBCMD::OutputLevel::DEFAULT:
        return Meatball::Config::OutputColors::defaultColor;
    case HayBCMD::OutputLevel::ECHO:
        return Meatball::Config::OutputColors::echoColor;
    case HayBCMD::OutputLevel::WARNING:
        return Meatball::Config::OutputColors::warningColor;
    case HayBCMD::OutputLevel::ERROR:
        return Meatball::Config::OutputColors::errorColor;
    }

    return Meatball::Config::OutputColors::defaultColor;
}