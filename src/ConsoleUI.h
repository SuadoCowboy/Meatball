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

            if (inputBox.focused && (IsKeyPressed(KEY_TAB) || IsKeyPressedRepeat(KEY_TAB))) {
                inputBox.onTextChange(inputBox.text); // to reset colors

                if (IsKeyDown(KEY_LEFT_SHIFT) && autoCompleteSelectedIdx != 0)
                    --autoCompleteSelectedIdx;
                
                else if (autoCompleteSelectedIdx) {
                    ++autoCompleteSelectedIdx;
                    // THE SELECTED INDEX WORKS DIFFERENT FROM COLOREDTEXT!!
                    // TODO: FIXME!
                    if (autoCompleteSelectedIdx > autoCompleteBox.coloredText.size())
                }
                
                std::string fullString = "";
                size_t idx = 0; // index of the text split by ' ' 
                size_t nextRealIndex = 0; // index of the next coloredText pair
                for (auto& pair: autoCompleteBox.coloredText) {
                    nextRealIndex++;
                    // if there were only one coloredText, it would still have a ' ', so idx start with 1
                    // this is just a way to know when the string was read completly
                    if (pair.first.back() == ' ') idx++;

                    if (idx-1 != autoCompleteSelectedIdx)
                        continue;
                    
                    pair.second = autoCompleteSelectedTextColor;
                    
                    nextRealIndex -= 2;
                    while (autoCompleteBox.coloredText[nextRealIndex].first.back() != ' ') {
                        autoCompleteBox.coloredText[nextRealIndex].second = autoCompleteSelectedTextColor;
                        
                        if (nextRealIndex == 0) break;
                        --nextRealIndex;
                    }
                    break;
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
    
        unsigned char autoCompleteSelectedIdx;
    };
}