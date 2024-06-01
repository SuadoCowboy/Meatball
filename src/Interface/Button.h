#pragma once

#include <string>

#include <raylib.h>

#include "Shared.h"

namespace Meatball {
    class Button
    {
    public:
        Button();
        Button(float x, float y, float width, float height);
        Button(Rectangle rect);
        
        void update();

        /// @brief draws text inside the button bounds
        void drawText();

        /// @brief draws text inside the button bounds but centered
        /// @param centerX if should center on x axis
        /// @param centerY if should center on y axis
        void drawTextCentered(bool centerX, bool centerY);
        
        /// @brief draws the button rect only.
        void drawRect();

        /// @brief draws a X inside the rect bounds
        void drawX();

        bool isHovered();

        const std::string& getText();

        void setText(std::string newText);

        VoidFunc onRelease, onHover;
        Rectangle rect;
        Color *color, *hoveredColor, *textColor, *hoveredTextColor; // colors that are equal in other interfaces
        Font *font;

    private:
        bool hovered;

        std::string text;
    };
}