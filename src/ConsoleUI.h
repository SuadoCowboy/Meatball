#pragma once

#include <string>
#include <memory>

#include <HayBCMD.h>

#include "Scene.h"
#include "Interface/DynamicPanel.h"
#include "Interface/Button.h"
#include "Interface/ScrollTextBox.h"
#include "Interface/InputTextBox.h"
#include "Utils/Utils.h"
#include "OutputColors.h"

#ifndef CONSOLEUI_OUTPUT_MAX_LINES
#define CONSOLEUI_OUTPUT_MAX_LINES 200
#endif

#ifndef CONSOLEUI_INPUT_MAX_HISTORY
#define CONSOLEUI_INPUT_MAX_HISTORY 30 // [0-255]
#endif

namespace Meatball {
    namespace Config {
        struct ConsoleUI {
            ConsoleUI();
            
            Font *mainFont; // used for input/output and (if exists)send button
            Font *labelFont; // used only for label

            Color autoCompleteColor; // color of the rect
            Color autoCompleteTextColor;
            Color autoCompleteHighlightedTextColor;
            Color autoCompleteSelectedTextColor;

            Color labelTextColor;

            const char* labelText;
        };
    }

    class ConsoleUIScene : public Scene {
    public:

        /// @param visible if scene is visible or not(only this class uses this)
        ConsoleUIScene(Rectangle rect, std::shared_ptr<Config::ConsoleUI> config, bool visible = true);

        /// @brief appends text to outputTextbox
        void print(const HayBCMD::OutputLevel &level, const std::string &text) {
            size_t length = text.size()-1;
            while (text[length] == '\n' && length != 0)
                --length;
            
            if (length != 0)
                outputBox.appendText(text.substr(0, length+1), outputLevelToOutputColor(level));
            
            while (outputBox.getContentHeight() > CONSOLEUI_OUTPUT_MAX_LINES*(unsigned int)outputBox.config->font->baseSize) {
                if (outputBox.getText().size() == 0) break;
                outputBox.popFront();
            }
        }

        void draw();
        void update();

        std::shared_ptr<Config::ConsoleUI> config;

        // Only console can appear in every scene so only it needs visible boolean.
        // The rest of the scenes will be handled by a class or something that says which one should be used.
        bool visible;
        
        DynamicPanel mainPanel;
        Button closeButton;
        ScrollTextBox outputBox;
        InputTextBox inputBox;
        
        std::vector<std::pair<std::string, Color>> autoCompleteText;

        // margin - the space between mainPanel border and objects close to it
        static unsigned char margin;

    private:
        size_t autoCompleteSelectedIdxBegin, autoCompleteSelectedIdxEnd;
        std::string inputBoxOriginalText; // the inputBox text that was used before selecting in auto complete

        std::string inputHistory[CONSOLEUI_INPUT_MAX_HISTORY];
        unsigned char inputHistorySize;
        unsigned char inputHistoryPos; // the position the user is when using inputHistory

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