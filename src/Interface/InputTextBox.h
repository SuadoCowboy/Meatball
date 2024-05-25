#pragma once

#include <string>

#include <raylib.h>

namespace Meatball {
    class InputTextBox {
    public:
        InputTextBox();
        InputTextBox(float x, float y, float width, float height, unsigned char fontSize);

        void draw();
        void update();

        unsigned char fontSize;

        Color color, textColor, cursorColor;
    private:
        Rectangle rect;

        std::string text;
        unsigned int cursorPos;
    };
}