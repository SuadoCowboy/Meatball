#pragma once

#include <raylib.h>

#include "Shared.h"

namespace Meatball {
    struct Panel {
        Panel();
        Panel(float x, float y, float width, float height);
        Panel(Rectangle rect);
        
        void draw();

        // events
        //VoidFunc onClick; // could be used to drag panel? or create another panel class that handles it

        Color color;
        Rectangle rect;
    };
}