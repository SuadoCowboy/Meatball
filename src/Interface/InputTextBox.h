#pragma once

#include <functional>
#include <cstring>
#include <memory>

#include <raylib.h>

#include "Shared.h"
#include "Config/Interface.h"

#ifndef INPUT_TEXT_BOX_TEXT_BUFFER_SIZE // should be in unsigned short range
#define INPUT_TEXT_BOX_TEXT_BUFFER_SIZE 1000
#endif

namespace Meatball {
    class InputTextBox {
    public:
        InputTextBox(std::shared_ptr<Config::InputTextBox> config);
        InputTextBox(const Rectangle &rect, std::shared_ptr<Config::InputTextBox> config);

        void draw();
        void update();
        
        unsigned short getTextMaxSize() {
            return INPUT_TEXT_BOX_TEXT_BUFFER_SIZE;
        }
        const char* getText();
        void setText(const char newText[INPUT_TEXT_BOX_TEXT_BUFFER_SIZE]) {
            strcpy(text, newText);
        };

        std::shared_ptr<Config::InputTextBox> config;
        
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