#pragma once

#include <string>

#include <raylib.h>

#include "../Shared.h"

namespace Meatball {

    class Button {
    public:
        Button() {}
        Button(const Rectangle& rect);
        
        void update();
        
        VoidFunc onRelease, onHover;
        Rectangle rect = {0.0f, 0.0f, 0.0f, 0.0f};

        bool hovered = false;
    };
}