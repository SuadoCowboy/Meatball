#pragma once

#include <memory>

#include <raylib.h>

#include "Shared.h"

namespace Meatball {
    namespace Config {
        struct ScrollBar {
            ScrollBar();

            Color barColor;
            Color barHoveredColor;
            Color thumbColor;
            Color thumbHoveredColor1;
            Color thumbHoveredColor2;
        };
    }

    namespace Defaults {
        extern std::shared_ptr<Config::ScrollBar> scrollBarConfig;
    }

    class ScrollBar {
    public:
        ScrollBar();
        /// @param rect the bar rect
        ScrollBar(const Rectangle& rect, bool visible = true);
        
        /// @param parentRect to check if the mouse is between parent's boundaries to use mouse wheel 
        void update(const Rectangle &parentRect);

        void draw();

        float getScrollValue() const;

        /// @param viewHeight basically, the height of the parent
        /// @param contentHeight the maximum height that could be reached
        void updateThumbHeight(float viewHeight, float contentHeight);

        void setPosition(float x, float y);
        /// @warning should call updateThumbHeight and update after calling this function or else things might break
        void setSize(float width, float height);
        
        const Rectangle &getRect();

        std::shared_ptr<Config::ScrollBar> config;
        
        bool visible;

        static unsigned char scrollSpeed;
    private:
        Rectangle rect;

        bool barHovered = false, thumbHovered = false;
        float thumbY = 0, thumbHeight;
        float scrollValue = 0; // (0.0 - 1.0) it's %
        bool dragging = false; // Whether the thumb is currently being dragged
        float dragOffsetY = 0;
    };
}