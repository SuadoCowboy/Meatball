#pragma once

#include <string>

#include <raylib.h>

#include "Shared.h"
#include "Config/Interface.h"

namespace Meatball {
    class TextButton {
    public:
        TextButton(Config::TextButton &config);
        TextButton(Rectangle rect, Config::TextButton &config);

        void update();

        bool isHovered();
        const std::string &getText();
        void setText(std::string &newText);

        VoidFunc onRelease, onHover;
        Config::TextButton &config;
        Rectangle rect;

    private:
        bool hovered;

        std::string text;
    };
}