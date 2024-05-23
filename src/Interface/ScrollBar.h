#pragma once

#include <raylib.h>

#include "Shared.h"

namespace Meatball {
    class ScrollBar {
    public:
        /// @param barRect the biggest rectangle that is drawn
        ScrollBar(Rectangle barRect, bool visible = true);

        void draw();
        void update();

        /// @brief the parent object should use this function if mouse is within the parent's bounds
        void updateWheelScroll();

        float getScrollHeight() const;
        short getScrollY() const;

        Color barColor, barHoveredColor, scrollColor, scrollHoveredColor1, scrollHoveredColor2;
        bool visible;

        Rectangle barRect;

        static unsigned char scrollSpeed;
    private:
        bool barHovered, scrollHovered;
        short scrollY;

        bool dragging;
    };
}