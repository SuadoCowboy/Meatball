#pragma once

//#include <functional>

#include <raylib.h>

namespace Meatball {
    struct Panel {
        Panel();
        Panel(float x, float y, float width, float height);
        Panel(Rectangle rect);
        
        void draw();

        // events
        //std::function<void()> onClick, onRelease; // could be used to drag panel? or create another panel class that handles it

        Color color;
        Rectangle rect;
    };
}