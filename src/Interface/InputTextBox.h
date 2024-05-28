#pragma once

#include <string>
#include <functional>

#include <raylib.h>

#include "Shared.h"

namespace Meatball {
    class InputTextBox {
    public:
        InputTextBox();
        InputTextBox(float x, float y, float width, float height, Font* font);

        void draw();
        void update();

        Font* font;

        Color color, textColor, cursorColor;
        
        std::string text;

        // onSend by default runs when KEY_ENTER/KEY_KP_ENTER is pressed
        std::function<void(const std::string&)> onSend, onTextChange;
    
        bool focused;
        unsigned int cursorPos;

    private:
        Rectangle rect;

        float offsetX;
    };
}