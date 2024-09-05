#pragma once

#include <raylib.h>

#include "../Shared.h"

namespace Meatball {
    namespace Config {
        struct ScrollBar {
            Color barColor = {15,15,15,255};
            Color barHoveredColor = {25,25,25,255};
            Color thumbColor = {30,30,30,255};
            Color thumbHoveredColor1 = {60,60,60,255};
            Color thumbHoveredColor2 = {90,90,90,255};
        };
    }

    namespace Defaults {
        extern Config::ScrollBar scrollBarConfig;
    }

    class ScrollBar {
    public:
        ScrollBar() {}
        /// @param rect the bar rect
        ScrollBar(const Rectangle& rect, bool visible = true);
        
        /// @param parentRect to check if the mouse is between parent's boundaries to use mouse wheel 
        void update(const Rectangle& parentRect);

        void draw();

        float getScrollValue() const;
        void setScrollValue(float value);

        /// @param viewHeight basically, the height of the parent
        /// @param contentHeight the maximum height that could be reached
        void updateThumbHeight(float viewHeight, float contentHeight);

        void setPosition(float x, float y);
        /// @warning should call updateThumbHeight and update after calling this function or else things might break
        void setSize(float width, float height);
        
        const Rectangle& getRect();

        Config::ScrollBar *config = &Defaults::scrollBarConfig;
        
        bool visible = true;

        static unsigned char scrollSpeed;
    
    private:
        Rectangle rect = {0.0f,0.0f,0.0f,0.0f};

        bool barHovered = false, thumbHovered = false;
        float scrollValue = 0.0f; // IT'S NOT BETWEEN 0 AND 1 AS I THOUGHT
        float thumbY = 0.0f, thumbHeight = 1.0f;
        bool dragging = false; // Whether the thumb is currently being dragged
        float dragOffsetY = 0.0f;
    };
}