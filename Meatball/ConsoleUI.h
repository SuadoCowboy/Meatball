#pragma once

#include "NodeUI.h"

namespace Meatball {
    class ConsoleUI : public NodeUI {
    public:
        ConsoleUI();
        ~ConsoleUI();

        void draw();
        void update();
        void handleInput();

        void onFocusGain();
        void onFocusLoss();
    };
}