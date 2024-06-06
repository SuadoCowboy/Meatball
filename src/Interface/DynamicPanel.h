#pragma once

#include <raylib.h>

#include "Shared.h"
#include "Config/Interface.h"

namespace Meatball {
    /// @brief a panel that can be resized and moved
    class DynamicPanel {
    public:
        DynamicPanel(Config::DynamicPanel &config);
        DynamicPanel(const Rectangle &rect, Config::DynamicPanel &config);

        /// @brief should only be updated if mouse is on it
        void update();

        Config::DynamicPanel &config;
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
        // wasHovered = 0b10000
        unsigned char conditions = 0;
    };
}