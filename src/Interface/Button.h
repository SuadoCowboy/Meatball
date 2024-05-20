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
        
        void update(VoidFunc onClick, VoidFunc onHover);

        /// @param text text to draw inside the button
        /// @param fontSize fontSize of the text
        void draw(std::string text, int fontSize);

        bool isHovered();

        Rectangle rect;
    
    private:
        bool hovered;
    };
}