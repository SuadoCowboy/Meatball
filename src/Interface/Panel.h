#pragma once

#include <raylib.h>

#include "Shared.h"

namespace Meatball {
    struct Panel {
        Panel(float x, float y, float width, float height);
        Panel(Rectangle rect);
        
        void draw();

        // events
        VoidFunc onClick, onHover;

        Color color;
        Rectangle rect;
    };
}