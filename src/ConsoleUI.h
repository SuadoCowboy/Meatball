#pragma once

#include "Scene.h"
#include "Interface/Panel.h"
#include "Interface/Button.h"
#include "Interface/ScrollTextBox.h"
#include "Interface/ColoredTextBox.h"
#include "Interface/InputTextBox.h"
#include "Utils/Utils.h"

#ifndef CONSOLEUI_OUTPUT_MAX_LINES
#define CONSOLEUI_OUTPUT_MAX_LINES 1000
#endif

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

            // TODO: make the logic be in a function and then we call 2 times, 1 for +1 and other for -1
            if (inputBox.focused && (IsKeyPressed(KEY_TAB) || IsKeyPressedRepeat(KEY_TAB)) && autoCompleteBox.coloredText.size() != 0) {
                // -1
                if (IsKeyDown(KEY_LEFT_SHIFT)) {
                    if (autoCompleteSelectedIdxBegin != 0) {
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

            outputBox.update();
            closeButton.update();

            while (outputBox.getContentHeight() > CONSOLEUI_OUTPUT_MAX_LINES*outputBox.font->baseSize) {
                outputBox.popFront();
            }
        }

        // Only console can appear in every scene so only it needs visible boolean.
        // The rest of the scenes will be handled by a class or something that says which one should be used.
        bool visible;
        
        Panel mainPanel;
        Button closeButton;
        ScrollTextBox outputBox;
        ColoredTextBox autoCompleteBox;
        InputTextBox inputBox;

        Color autoCompleteHighlightTextColor, autoCompleteTextColor, autoCompleteSelectedTextColor;

        // margin - the space between mainPanel border and objects close to it
        static unsigned char margin;
    
        size_t autoCompleteSelectedIdxBegin, autoCompleteSelectedIdxEnd;
        std::string inputBoxOriginalText; // the inputBox text that was used before selecting in auto complete
    };
}