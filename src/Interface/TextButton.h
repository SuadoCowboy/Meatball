#pragma once

#include <string>

#include <raylib.h>

#include "../Shared.h"

namespace Meatball {
    namespace Config {
        struct TextButton {
            Font* font = nullptr;

            Color color = BLACK;
            Color textColor = WHITE;

            Color hoveredColor = WHITE;
            Color hoveredTextColor = BLACK;
        };
    }

    namespace Defaults {
        extern Meatball::Config::TextButton textButtonConfig;
    }

    class TextButton {
    public:
        TextButton();
        TextButton(const Rectangle& rect);

        void update();

        /// @brief draws text centered on the button
        void drawText();

        bool isHovered();

        Config::TextButton *config = &Defaults::textButtonConfig;

        VoidFunc onRelease, onHover;
        Rectangle rect = {0.0f,0.0f,0.0f,0.0f};

        std::string text;
        unsigned short fontSize = 0;
    private:
        bool hovered = false;
    };
}