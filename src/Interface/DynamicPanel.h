#pragma once

#include <raylib.h>

#include "../Shared.h"

namespace Meatball {
    namespace Config {
        struct DynamicPanel {
            DynamicPanel();

            Color color;
            Vector2 minSize;
            float grabHeight;
        };
    }

    namespace Defaults {
        extern Config::DynamicPanel dynamicPanelConfig;
    }

    /// @brief a panel that can be resized and moved
    class DynamicPanel {
    public:
        DynamicPanel();
        DynamicPanel(const Rectangle& rect);

        void update();

        /// @brief whether is dragging/resizing/hovered
        bool isAnyConditionActive();

        Config::DynamicPanel *config = &Defaults::dynamicPanelConfig;
        Rectangle rect;

        // TODO: grabHeight proportional to screen size or window size

        VoidFunc onResize, onMove;
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