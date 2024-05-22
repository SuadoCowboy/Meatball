#pragma once

#include "Scene.h"
#include "Interface/Panel.h"
#include "Interface/Button.h"

namespace Meatball {
    class ConsoleUIScene : public Scene {
    public:
        /// @param x mainPanel x
        /// @param y mainPanel y
        /// @param width mainPanel width
        /// @param height mainPanel height
        /// @param visible if scene is visible or not(only this class uses this)
        ConsoleUIScene(float x, float y, float width, float height, bool visible = true);

        void draw();
        void update();

        // Only console can appear in every scene so only it needs visible boolean.
        // The rest of the scenes will be handled by a class or something that says which one should be used.
        bool visible;

        Panel mainPanel;
        Button closeButton;
        Button sendButton;

        // margin - the space between mainPanel border and objects close to it
        static unsigned char margin;
    };
}