#pragma once

#include "NodeUI.h"

#include <raylib.h>

namespace Meatball {
    class ConsoleUI : public NodeUI {
    public:
        ConsoleUI();
        ConsoleUI(float x, float y, float width, float height, Color color = { 22, 22, 22, 255 });
        ~ConsoleUI();

        void draw();
        void update();
        void handleInput();

        void onFocusGain();
        void onFocusLoss();
    
    private:
        Rectangle mainPanel;
        Color mainPanelColor;
    };
}