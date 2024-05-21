#pragma once

#include <string>

#include <raylib.h>

#include "Shared.h"

namespace Meatball {
    class Button
    {
    public:
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

        //bool isCollidedX/Rect(); // are those needed?

        bool isHovered();

        VoidFunc onClick, onHover;

        Rectangle rect;
        Color color, hoveredColor, textColor, textHoveredColor;
        
        const std::string& getText();
        const unsigned char& getFontSize();

        /// @param fontSize 0 to maintain current fontSize else change fontSize
        void setText(std::string newText, unsigned char newFontSize = 0);
    
    private:
        bool hovered;

        std::string text;
        unsigned char fontSize;
    };
}