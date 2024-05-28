#pragma once

#include <raylib.h>

namespace Meatball {
    /// @brief a panel that can be resized and moved
    class DynamicPanel {
    public:
        DynamicPanel();
        DynamicPanel(Rectangle rect, Vector2 minSize = {1,1});
        DynamicPanel(float x, float y, float width, float height, Vector2 minSize = {1,1});

        /// @brief should only be updated if mouse is on it
        void update();
        void draw();

        Color color;
        Rectangle rect;
        Vector2 minSize; // width & height
    private:
        Vector2 offset; // this is used for dragging and resizing
        bool dragging, resizing, resizingFromN, resizingFromW, wasHovered;
    };
}