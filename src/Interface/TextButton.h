#pragma once

#include <string>

#include <raylib.h>

#include "../Shared.h"

namespace Meatball {
    namespace Config {
        struct TextButton {
            TextButton();

            Font* font;
            unsigned short fontSize;

            Color color;
            Color textColor;

            Color hoveredColor;
            Color hoveredTextColor;
        };
    }

    namespace Defaults {
        extern Meatball::Config::TextButton textButtonConfig;
    }

    class TextButton {
    public:
        TextButton();
        TextButton(Rectangle rect);

        void update();

        /// @brief draws text centered on the button
        void drawText();

        bool isHovered();
        
        const std::string &getText();
        void setText(const std::string &newText);

        Config::TextButton *config = &Defaults::textButtonConfig;

        VoidFunc onRelease, onHover;
        Rectangle rect;

    private:
        bool hovered;

        std::string text;
    };
}