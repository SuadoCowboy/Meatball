#pragma once

#include <raylib.h>

namespace Meatball {
    struct Panel {
        Panel();
        Panel(float x, float y, float width, float height);
        Panel(Rectangle rect);
        
        void draw();

        Color color;
        Rectangle rect;
    };
}