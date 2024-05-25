#pragma once

#include <raylib.h>

#include "Shared.h"

namespace Meatball {
    class ScrollBar {
    public:
        ScrollBar();
        /// @param barRect the biggest rectangle that is drawn
        ScrollBar(Rectangle barRect, bool visible = true);
        
        /// @param parentRect to check if the mouse is between parent's boundaries to use mouse wheel 
        void update(const Rectangle& parentRect);

        void draw();

        float getScrollValue() const;

        /// @param viewHeight basically, the height of the parent
        /// @param contentHeight the maximum height that could be reached
        void updateThumbHeight(float viewHeight, float contentHeight);

        Color barColor, barHoveredColor, thumbColor, thumbHoveredColor1, thumbHoveredColor2;
        bool visible;

        Rectangle barRect;

        static unsigned char scrollSpeed;
    private:
        bool barHovered, thumbHovered;
        float thumbY, thumbHeight;
        float scrollValue; // (0.0 - 1.0) it's %

        bool dragging; // Whether the thumb is currently being dragged
        float dragOffsetY;
    };
}