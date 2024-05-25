#pragma once

#include "Scene.h"
#include "Interface/Panel.h"
#include "Interface/Button.h"
#include "Interface/TextBox.h"
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
        ConsoleUIScene(float x, float y, float width, float height, unsigned char fontSize, bool visible = true);

        /// @brief appends text to outputTextbox
        void print(const std::string& message);

        void draw();
        void update() {
            if (!visible) return;

            outputBox.update();
            sendButton.update();
            closeButton.update();

            while (outputBox.getContentHeight() > CONSOLEUI_OUTPUT_MAX_LINES*outputBox.getFontSize()) {
                outputBox.popFront();
            }
        }

        // Only console can appear in every scene so only it needs visible boolean.
        // The rest of the scenes will be handled by a class or something that says which one should be used.
        bool visible;

        Panel mainPanel;
        Button closeButton;
        Button sendButton;
        TextBox outputBox;

        // margin - the space between mainPanel border and objects close to it
        static unsigned char margin;
    };
}