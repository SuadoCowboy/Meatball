#pragma once

#include <functional>
#include <string>

#include <raylib.h>

#include "../Shared.h"

namespace Meatball {
    namespace Config {
        struct InputTextBox {
            InputTextBox();

            Font* font;
            unsigned short fontSize;

            Color color, textColor, cursorColor, selectionColor;
        };
    }

    namespace Defaults {
        extern Config::InputTextBox inputTextBoxConfig;
    }

    class InputTextBox {
    public:
        InputTextBox();
        InputTextBox(const Rectangle& rect);

        void draw();
        void update();

        Config::InputTextBox *config = &Defaults::inputTextBoxConfig;
        
        // onSend by default runs when KEY_ENTER/KEY_KP_ENTER is pressed
        std::function<void(const std::string&)> onSend, onTextChange;
        
        bool focused = false;
        size_t cursorPos = 0;

        Rectangle rect;

        const std::string& getText() const;
        /// @return false if text size is higher than allowed
        bool setText(const std::string& text);

        // textMaxSize range is the unsigned short range from 0 to max unsigned short MINUS ONE;
        unsigned short textMaxSize = 1000;
    private:
        std::string text = "";

        float offsetX = 0;

        bool mousePressed = false;

        unsigned short selectedTextStartIdx = 0;
        unsigned short selectedTextFinalIdx = 0;
    };
}