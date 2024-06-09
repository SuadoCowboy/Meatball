#include "OutputColors.h"

std::shared_ptr<Color> Meatball::Config::OutputColors::defaultColor = std::make_shared<Color>(WHITE);
// Light blue
std::shared_ptr<Color> Meatball::Config::OutputColors::echoColor = std::make_shared<Color>((Color){173, 216, 230, 255});
// Light yellow
std::shared_ptr<Color> Meatball::Config::OutputColors::warningColor = std::make_shared<Color>((Color){253, 250, 114, 255});
std::shared_ptr<Color> Meatball::Config::OutputColors::errorColor = std::make_shared<Color>(RED);

const std::shared_ptr<Color>& Meatball::outputLevelToOutputColor(const HayBCMD::OutputLevel& level) {
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