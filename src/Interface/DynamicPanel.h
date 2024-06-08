#pragma once

#include <memory>

#include <raylib.h>

#include "Shared.h"

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
        static std::shared_ptr<Config::DynamicPanel> dynamicPanelConfig;
    }

    /// @brief a panel that can be resized and moved
    class DynamicPanel {
    public:
        DynamicPanel();
        DynamicPanel(const Rectangle &rect);

        void update();

        /// @brief whether is dragging/resizing/hovered
        bool isAnyConditionActive();

        std::shared_ptr<Config::DynamicPanel> config = Defaults::dynamicPanelConfig;
        Rectangle rect;

        // TODO: grabHeight proportional to screen size or window size

        VoidFunc onResize, onMove;
        VoidFunc onResizeStop; // when resizing is just set to false

    private:
        Vector2 offset = {0,0}; // this is used for dragging and resizing

        // dragging = 0b00001
        // resizing = 0b00010
        // resizingFromN = 0b00100
        // resizingFromW = 0b01000
        // hovered = 0b10000
        unsigned char conditions = 0;
    };
}