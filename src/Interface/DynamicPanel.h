#pragma once

#include <raylib.h>

#include "../Shared.h"

namespace Meatball {
    /// @brief a panel that can be resized and moved
    class DynamicPanel {
    public:
        DynamicPanel() {}
        DynamicPanel(const Rectangle& rect);

        void update();

        /// @brief whether is dragging/resizing/hovered
        bool isAnyConditionActive();
        
        Rectangle rect = {0.0f, 0.0f, 0.0f, 0.0f};

        Vector2 minSize = {8,10};
        float grabHeight = 2.0f;

        // TODO: grabHeight proportional to screen size or window size

        // should also call onMove in the end of it
        VoidFunc onResize;
        VoidFunc onMove;
        VoidFunc onResizeStop; // when resizing is just set to false

    private:
        Vector2 offset = {0,0}; // this is used for dragging and resizing

        /*
        dragging = 1
        resizing = 2
        resizingFromN = 4
        resizingFromW = 8
        hovered = 16
        */
        unsigned char conditionFlags = 0;
    };
}