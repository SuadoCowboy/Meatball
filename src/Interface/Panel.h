#pragma once

#include <raylib.h>

#include "Shared.h"

#define PANEL_FOCUS_LEVEL (unsigned char)1

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