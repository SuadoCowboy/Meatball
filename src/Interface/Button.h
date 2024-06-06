#pragma once

#include <string>

#include <raylib.h>

#include "Shared.h"
#include "Config/Interface.h"

namespace Meatball {
    class Button {
    public:
        Button(Config::Button &config);
        Button(Rectangle &rect, Config::Button &config);
        
        void update();
        bool isHovered();

        VoidFunc onRelease, onHover;
        Config::Button &config;
        Rectangle rect;

    private:
        bool hovered;
    };
}