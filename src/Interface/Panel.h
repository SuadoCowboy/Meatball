#pragma once

#include <raylib.h>

#include "Shared.h"

/*
0 = scene
1 = panels
2 = buttons?
3 = input_box??
4 = ??????
*/
#define PANEL_Z_LEVEL (unsigned char)1

namespace Meatball {
    struct Panel {
        Panel(float x, float y, float width, float height);
        Panel(Rectangle rect);

        // TODO: Those parameters being passed ON EACH OBJECT might not be ideal. Find a better way.
        void update(VoidFunc onClick, VoidFunc onHover);
        void draw();

        Color color;
        Rectangle rect;
    };
}