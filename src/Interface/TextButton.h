#pragma once

#include <string>
#include <memory>

#include <raylib.h>

#include "Shared.h"

namespace Meatball {
    namespace Config {
        struct TextButton {
            TextButton();

            std::shared_ptr<Font> font;

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

        std::shared_ptr<Config::TextButton> config;
        VoidFunc onRelease, onHover;
        Rectangle rect;

    private:
        bool hovered;

        std::string text;
    };
}