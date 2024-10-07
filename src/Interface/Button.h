#pragma once

#include <string>

#include <raylib.h>

#include "../Shared.h"

namespace Meatball {

    class Button {
    public:
        Button() {}
        Button(const Rectangle& rect);

        void onMouseMove(const Vector2& mousePosition);
        
        VoidFunc onMouseRelease, onHover, onUnhover;
        Rectangle rect = {0.0f, 0.0f, 0.0f, 0.0f};

        bool hovered = false;
    };
}