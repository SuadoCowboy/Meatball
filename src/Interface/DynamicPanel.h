#pragma once

#include <raylib.h>

namespace Meatball {
    class DynamicPanel {
    public:
        DynamicPanel();
        DynamicPanel(float x, float y, float width, float height);
        DynamicPanel(Rectangle rect);

        void draw();

        Color color;
        Rectangle rect;
    private:
    };
}