#pragma once

#include <functional>
#include <cstring>
#include <memory>

#include <raylib.h>

#include "Shared.h"

#ifndef INPUT_TEXT_BOX_TEXT_BUFFER_SIZE // should be in unsigned short range
#define INPUT_TEXT_BOX_TEXT_BUFFER_SIZE 1000
#endif

namespace Meatball {
    namespace Config {
        struct InputTextBox {
            InputTextBox();

            Font *font;
            Color color, textColor, cursorColor, selectionColor;
        };
    }

    namespace Defaults {
        static std::shared_ptr<Config::InputTextBox> inputTextBoxConfig;
    }

    class InputTextBox {
    public:
        InputTextBox();
        InputTextBox(const Rectangle &rect);

        void draw();
        void update();
        
        static constexpr unsigned short getTextMaxSize() {
            return INPUT_TEXT_BOX_TEXT_BUFFER_SIZE;
        }

        const char* getText();
        void setText(const char newText[INPUT_TEXT_BOX_TEXT_BUFFER_SIZE]) {
            strcpy(text, newText);
        };

        std::shared_ptr<Config::InputTextBox> config = Defaults::inputTextBoxConfig;
        
        // onSend by default runs when KEY_ENTER/KEY_KP_ENTER is pressed
        std::function<void(const char*)> onSend, onTextChange;
        
        bool focused = false;
        size_t cursorPos = 0;

        Rectangle rect;

    private:
        float offsetX = 0;

        bool mousePressed;

        char text[INPUT_TEXT_BOX_TEXT_BUFFER_SIZE];
        unsigned short selectedTextStartIdx = INPUT_TEXT_BOX_TEXT_BUFFER_SIZE;
        unsigned short selectedTextFinalIdx = INPUT_TEXT_BOX_TEXT_BUFFER_SIZE;
    };
}