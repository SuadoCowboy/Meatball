#pragma once

#include <memory>

#include <raylib.h>

#include "Shared.h"
#include "Config/Interface.h"

namespace Meatball {
    /// @brief a panel that can be resized and moved
    class DynamicPanel {
    public:
        DynamicPanel(std::shared_ptr<Config::DynamicPanel> config);
        DynamicPanel(const Rectangle &rect, std::shared_ptr<Config::DynamicPanel> config);

        void update();

        /// @brief whether is dragging/resizing/hovered
        bool isAnyConditionActive();

        std::shared_ptr<Config::DynamicPanel> config;
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