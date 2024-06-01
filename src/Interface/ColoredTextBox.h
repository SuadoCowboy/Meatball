#pragma once

#include <string>
#include <vector>
#include <utility>

#include <raylib.h>

#include "Shared.h"

namespace Meatball {
    /// @brief a one-line TextBox that the text can change its color on specific parts
    class ColoredTextBox {
    public:
        ColoredTextBox();
        ColoredTextBox(float x, float y, float width, float height, Font *font);
    
        void pushText(const std::string& text, Color color);

        void draw();

        std::vector<std::pair<std::string, Color>> coloredText;
        
        Rectangle rect;

        Font *font;
        Color color;
    };
}