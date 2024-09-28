#include "OutputColors.h"

Color Meatball::Config::OutputColors::defaultColor = WHITE;
// Light blue
Color Meatball::Config::OutputColors::echoColor = {173, 216, 230, 255};
// Light yellow
Color Meatball::Config::OutputColors::warningColor = {253, 250, 114, 255};
Color Meatball::Config::OutputColors::errorColor = RED;

Color& Meatball::outputLevelToOutputColor(const SweatCI::OutputLevel& level) {
    switch (level) {
    case SweatCI::OutputLevel::DEFAULT:
        return Meatball::Config::OutputColors::defaultColor;
    case SweatCI::OutputLevel::ECHO:
        return Meatball::Config::OutputColors::echoColor;
    case SweatCI::OutputLevel::WARNING:
        return Meatball::Config::OutputColors::warningColor;
    case SweatCI::OutputLevel::ERROR:
        return Meatball::Config::OutputColors::errorColor;
    }

    return Meatball::Config::OutputColors::defaultColor;
}