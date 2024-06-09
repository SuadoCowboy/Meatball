#pragma once

#include <functional>
#include <memory>
#include <string>

#include <raylib.h>

#include "Shared.h"

#ifndef INPUT_TEXT_BOX_TEXT_BUFFER_SIZE
#define INPUT_TEXT_BOX_TEXT_BUFFER_SIZE 10 // unsigned short range but the max number is itself-1 TODO: change this to a good value lol
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
        extern std::shared_ptr<Config::InputTextBox> inputTextBoxConfig;
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

        std::shared_ptr<Config::InputTextBox> config = Defaults::inputTextBoxConfig;
        
        // onSend by default runs when KEY_ENTER/KEY_KP_ENTER is pressed
        std::function<void(std::string&)> onSend, onTextChange;
        
        bool focused = false;
        size_t cursorPos = 0;

        Rectangle rect;

        std::string text = "";

    private:
        float offsetX = 0;

        bool mousePressed = false;

        unsigned short selectedTextStartIdx = 0;
        unsigned short selectedTextFinalIdx = 0;
    };
}