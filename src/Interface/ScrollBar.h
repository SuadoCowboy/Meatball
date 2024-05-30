#pragma once

#include <raylib.h>

#include "Shared.h"

namespace Meatball {
    class ScrollBar {
    public:
        ScrollBar();
        /// @param rect the bar rect
        ScrollBar(Rectangle rect, bool visible = true);
        
        /// @param parentRect to check if the mouse is between parent's boundaries to use mouse wheel 
        void update(const Rectangle& parentRect);

        void draw();

        float getScrollValue() const;

        /// @param viewHeight basically, the height of the parent
        /// @param contentHeight the maximum height that could be reached
        void updateThumbHeight(float viewHeight, float contentHeight);

        Color barColor, barHoveredColor, thumbColor, thumbHoveredColor1, thumbHoveredColor2;
        bool visible;

        void setPosition(float x, float y);
        /// @warning should call updateThumbHeight and update after calling this function or else things might break
        void setSize(float width, float height);
        
        const Rectangle& getRect();

        static unsigned char scrollSpeed;
    private:
        Rectangle rect;

        bool barHovered, thumbHovered;
        float thumbY, thumbHeight;
        float scrollValue; // (0.0 - 1.0) it's %

        bool dragging; // Whether the thumb is currently being dragged
        float dragOffsetY;
    };
}