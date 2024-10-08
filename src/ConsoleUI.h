#pragma once

#include <string>

#include <SweatCI.h>

#include "Interface/DynamicPanel.h"
#include "Interface/Button.h"
#include "Interface/ScrollBox.h"
#include "Interface/InputTextBox.h"
#include "Interface/Theme.h"
#include "Utils/Utils.h"
#include "OutputColors.h"


#ifndef CONSOLEUI_OUTPUT_MAX_LINES
#define CONSOLEUI_OUTPUT_MAX_LINES 200
#endif

#ifndef CONSOLEUI_INPUT_MAX_HISTORY
#define CONSOLEUI_INPUT_MAX_HISTORY 30 // [0-255]
#endif

namespace Meatball {
    class ConsoleUI {
    public:
        /// @brief makes a empty ConsoleUI
        ConsoleUI(const Rectangle& rect, bool visible = false);

        virtual ~ConsoleUI() {};

        /// @brief appends text to outputTextbox
        void print(const SweatCI::OutputLevel& level, const std::string& text) {
            size_t length = text.size();
            while (text[length-1] == '\n' && length != 0)
                --length;
            
            if (length != 0)
                outputBox.appendText(text.substr(0, length), outputFont, outputLevelToOutputColor(level));
            
            while (outputBox.getContentHeight() > CONSOLEUI_OUTPUT_MAX_LINES*(unsigned int)outputBox.fontSize) {
                if (outputBox.getText().size() == 0) break;
                outputBox.popFront();
            }
        }

        void draw() const;

        /// @brief should be used when window is resized or render screen or even font size
        /// @param ratioWidth renderWidthNow/renderWidthBefore
        /// @param ratioHeight renderHeightNow/renderHeightBefore
        void onResize(float ratioWidth, float ratioHeight);

        void onCharPress(int codepoint);
        void onKeyboardPress(int key, bool isRepeat);
        void onMousePress(int button);
        void onMouseRelease(int button);
        void onMouseMove(const Vector2& mousePosition);

        // Only console can appear in every scene so only it needs visible boolean.
        // The rest of the scenes will be handled by a class or something that says which one should be used.
        bool visible = false;
        
        Font outputFont, inputFont, labelFont;

        DynamicPanel mainPanel;
        Button closeButton;
        ScrollBox outputBox;
        InputTextBox inputBox;

        std::string labelText = "";

        Color autoCompleteColor = BLACK;
        Color autoCompleteTextColor = BLACK;
        Color autoCompleteHighlightedTextColor = BLACK;
        Color autoCompleteSelectedTextColor = BLACK;

        Color labelTextColor = BLACK;

        Color backgroundColor = BLACK;

        ScrollBoxTheme outputBoxTheme;
        InputTextBoxTheme inputBoxTheme;
        ButtonTheme closeButtonTheme;
        ScrollBarTheme outputScrollBarTheme;

        // margin - the space between mainPanel border and objects close to it
        unsigned char margin = 4;

    private:
        std::vector<ColoredText> autoCompleteText;
        size_t autoCompleteSelectedIdxBegin = 0, autoCompleteSelectedIdxEnd = 0;
        
        std::string inputBoxOriginalText; // the inputBox text that was used before selecting in auto complete
        std::string inputHistory[CONSOLEUI_INPUT_MAX_HISTORY];
        unsigned char inputHistorySize = 0;
        unsigned char inputHistoryPos = 0; // the position the user is when using inputHistory

        void addToInputHistory(const std::string &string) {
            if (inputHistorySize != 0 && inputHistory[inputHistorySize-1].compare(string) == 0) {
                inputHistoryPos = inputHistorySize;
                return;
            }
            
            if (inputHistorySize >= CONSOLEUI_INPUT_MAX_HISTORY) {
                for (unsigned char i = 1; i < inputHistorySize; i++) {
                    inputHistory[i-1] = inputHistory[i];
                }
                --inputHistorySize;
            }

            inputHistory[inputHistorySize] = string;
            ++inputHistorySize;

            inputHistoryPos = inputHistorySize;
        }
    };
}