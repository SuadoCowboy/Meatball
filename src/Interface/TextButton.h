#pragma once

#include <string>

#include <raylib.h>

#include "../Shared.h"

namespace Meatball {
    class TextButton {
    public:
        TextButton() {}
        TextButton(const Rectangle& rect, unsigned short fontSize);

        void onMouseMove(const Vector2& mousePosition);

        /// @brief draws text centered on the button
        void drawText(const Font& font, const Color& textColor, const Color& hoveredTextColor) const;

        VoidFunc onMouseRelease, onHover, onUnhover;
        Rectangle rect = {0.0f,0.0f,0.0f,0.0f};

        std::string text;
        unsigned short fontSize = 10;

        bool hovered = false;
    };
}