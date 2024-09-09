#pragma once

#include <string>

#include <raylib.h>

#include "../Shared.h"

namespace Meatball {
    namespace Config {
        struct Button {
            Color color = BLACK;
            Color hoveredColor = WHITE;
        };
    }

    namespace Defaults {
        extern Config::Button buttonConfig;
    }

    class Button {
    public:
        Button() {}
        Button(const Rectangle& rect);
        
        void update();

        Config::Button *config =  &Defaults::buttonConfig;
        
        VoidFunc onRelease, onHover;
        Rectangle rect = {0.0f,0.0f,0.0f,0.0f};

        bool hovered = false;
    };
}