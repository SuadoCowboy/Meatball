#pragma once

#include <string>

#include <raylib.h>

#include "../Shared.h"

namespace Meatball {
    namespace Config {
        struct Button {
            Button();

            Color color;
            Color hoveredColor;
        };
    }

    namespace Defaults {
        extern Config::Button buttonConfig;
    }

    class Button {
    public:
        Button();
        Button(const Rectangle& rect);
        
        void update();
        bool isHovered();

        Config::Button *config =  &Defaults::buttonConfig;
        
        VoidFunc onRelease, onHover;
        Rectangle rect;

    private:
        bool hovered;
    };
}