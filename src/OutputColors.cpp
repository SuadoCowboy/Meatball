#include "OutputColors.h"

Color Meatball::OutputColors::defaultColor = WHITE;
// Light blue
Color Meatball::OutputColors::echoColor = {173, 216, 230, 255};
// Light yellow
Color Meatball::OutputColors::warningColor = {253, 250, 114, 255};
Color Meatball::OutputColors::errorColor = RED;

Color& Meatball::outputLevelToOutputColor(const SweatCI::OutputLevel& level) {
    switch (level) {
    case SweatCI::OutputLevel::DEFAULT:
        return Meatball::OutputColors::defaultColor;
    case SweatCI::OutputLevel::ECHO:
        return Meatball::OutputColors::echoColor;
    case SweatCI::OutputLevel::WARNING:
        return Meatball::OutputColors::warningColor;
    case SweatCI::OutputLevel::ERROR:
        return Meatball::OutputColors::errorColor;
    }

    return Meatball::OutputColors::defaultColor;
}