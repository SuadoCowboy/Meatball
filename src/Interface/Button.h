#pragma once

#include <string>
#include <memory>

#include <raylib.h>

#include "Shared.h"

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
        Button(Rectangle& rect);
        
        void update();
        bool isHovered();

        std::shared_ptr<Config::Button> config;
        
        VoidFunc onRelease, onHover;
        Rectangle rect;

    private:
        bool hovered;
    };
}