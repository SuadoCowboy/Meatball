#pragma once

#include <string>

#include "Scene.h"
#include "Interface/DynamicPanel.h"
#include "Interface/Button.h"
#include "Interface/ScrollTextBox.h"
#include "Interface/ColoredTextBox.h"
#include "Interface/InputTextBox.h"
#include "Utils/Utils.h"

#ifndef CONSOLEUI_OUTPUT_MAX_LINES
#define CONSOLEUI_OUTPUT_MAX_LINES 1000
#endif

#ifndef CONSOLEUI_INPUT_MAX_HISTORY
#define CONSOLEUI_INPUT_MAX_HISTORY 30 // [0-255]
#endif

static void handleInputHistoryPos(Meatball::InputTextBox& inputBox, std::string* inputHistory, const unsigned char& inputHistorySize, unsigned char& inputHistoryPos) {
    if ((IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_UP)) && inputHistoryPos != 0)
        --inputHistoryPos;
    
    else if ((IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN)) && inputHistoryPos != inputHistorySize-1)
        ++inputHistoryPos;
    
    else return;
    
    inputBox.text = inputHistory[inputHistoryPos];
    inputBox.cursorPos = inputBox.text.size();
    inputBox.onTextChange(inputBox.text);
}

namespace Meatball {
    class ConsoleUIScene : public Scene {
    public:
        /// @param x mainPanel x
        /// @param y mainPanel y
        /// @param width mainPanel width
        /// @param height mainPanel height
        /// @param visible if scene is visible or not(only this class uses this)
        ConsoleUIScene(float x, float y, float width, float height, Font* font, bool visible = true);

        /// @brief appends text to outputTextbox
        void print(const std::string& message);

        void draw();
        void update() {
            if (!visible) return;

            inputBox.update();
            
            if (inputBox.focused) {
                if ((IsKeyPressed(KEY_TAB) || IsKeyPressedRepeat(KEY_TAB)) && autoCompleteBox.coloredText.size() != 0) {
                    // -1
                    if (IsKeyDown(KEY_LEFT_SHIFT) && autoCompleteSelectedIdxBegin != 0) {
                        inputBox.onTextChange(inputBoxOriginalText);

                        autoCompleteSelectedIdxEnd = autoCompleteSelectedIdxBegin-1;
                        
                        size_t idx = autoCompleteSelectedIdxEnd;
                        autoCompleteBox.coloredText[idx].second = autoCompleteSelectedTextColor;
                        std::string newText = autoCompleteBox.coloredText[idx].first;
                        
                        --idx;
                        while (autoCompleteBox.coloredText[idx].first.back() != ' ') {
                            autoCompleteBox.coloredText[idx].second = autoCompleteSelectedTextColor;
                            newText = autoCompleteBox.coloredText[idx].first+newText;
                            if (idx == 0) break;
                            --idx;
                        }
                        
                        newText.pop_back();
                        inputBox.text = newText;
                        inputBox.cursorPos = inputBox.text.size();
                        autoCompleteSelectedIdxBegin = idx == 0? 0 : idx+1;
                    }
                    
                    // +1
                    else if (autoCompleteSelectedIdxEnd != autoCompleteBox.coloredText.size()-1) {
                        inputBox.onTextChange(inputBoxOriginalText);
                        
                        autoCompleteSelectedIdxBegin = autoCompleteSelectedIdxEnd+1;
                        if (autoCompleteSelectedIdxEnd == 0) autoCompleteSelectedIdxBegin = 0;
                        
                        size_t idx = autoCompleteSelectedIdxBegin;
                        autoCompleteBox.coloredText[idx].second = autoCompleteSelectedTextColor;
                        std::string newText = autoCompleteBox.coloredText[idx].first;
                        
                        while (autoCompleteBox.coloredText[idx].first.back() != ' ') {
                            ++idx;
                            autoCompleteBox.coloredText[idx].second = autoCompleteSelectedTextColor;
                            newText += autoCompleteBox.coloredText[idx].first;
                        }
                        
                        newText.pop_back();
                        inputBox.text = newText;
                        inputBox.cursorPos = inputBox.text.size();
                        autoCompleteSelectedIdxEnd = idx;
                    }
                }

                else if (inputHistorySize != 0)
                    handleInputHistoryPos(inputBox, inputHistory, inputHistorySize, inputHistoryPos);
            }

            outputBox.update();
            closeButton.update();

            while (outputBox.getContentHeight() > CONSOLEUI_OUTPUT_MAX_LINES*(float)outputBox.font->baseSize) {
                if (outputBox.getText().size() == 0) break;
                outputBox.popFront();
            }

            mainPanel.update();
        }

        // Only console can appear in every scene so only it needs visible boolean.
        // The rest of the scenes will be handled by a class or something that says which one should be used.
        bool visible;
        
        DynamicPanel mainPanel;
        Button closeButton;
        ScrollTextBox outputBox;
        ColoredTextBox autoCompleteBox;
        InputTextBox inputBox;

        Color autoCompleteHighlightedTextColor, autoCompleteTextColor, autoCompleteSelectedTextColor;

        // margin - the space between mainPanel border and objects close to it
        static unsigned char margin;
        
        size_t autoCompleteSelectedIdxBegin, autoCompleteSelectedIdxEnd;
        std::string inputBoxOriginalText; // the inputBox text that was used before selecting in auto complete

    
    private:
        std::string inputHistory[CONSOLEUI_INPUT_MAX_HISTORY];
        unsigned char inputHistorySize;
        unsigned char inputHistoryPos; // the position the user is when using inputHistory

        void addToInputHistory(const std::string& string) {
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