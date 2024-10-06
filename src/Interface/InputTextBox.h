#pragma once

#include <functional>
#include <string>

#include <raylib.h>

#include "../Shared.h"

namespace Meatball {
    class InputTextBox {
    public:
        InputTextBox();
        InputTextBox(const Rectangle& rect, unsigned short fontSize);

        void draw(const Font& font, const Color& textColor, const Color& cursorColor, const Color& selectionColor) const;
        void update(const Font& font);
        
        // onSend by default runs when KEY_ENTER/KEY_KP_ENTER is pressed
        std::function<void(const std::string&)> onSend, onTextChange;
        
        bool focused = false;
        size_t cursorPos = 0;

        Rectangle rect = {0.0f,0.0f,0.0f,0.0f};

        const std::string& getText() const;
        /// @return false if text size is higher than allowed
        bool setText(const std::string& text);

        // textMaxSize range is the unsigned short range from 0 to max unsigned short MINUS ONE;
        unsigned short textMaxSize = 1000;

        unsigned short fontSize = 10;
    private:
        std::string text = "";

        float offsetX = 0;

        bool mousePressed = false;

        unsigned short selectedTextStartIdx = 0;
        unsigned short selectedTextFinalIdx = 0;
    };
}