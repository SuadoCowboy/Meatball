#pragma once

#include <string>
#include <list>

#include <raylib.h>

#include "ScrollBar.h"

namespace Meatball {
    /// @brief basically, a rect being used to draw text but with scrolling
    /// @note it also have a scrollable bar when text is higher than rect.height
    /// @note if you're looking for text input see TextInputBox instead
    class ScrollTextBox {
    public:
        ScrollTextBox();
        ScrollTextBox(float x, float y, float width, float height, Font *font);

        const Rectangle& getRect() const;
        const unsigned int& getContentHeight() const;
        ScrollBar& getScrollBar();

        void appendText(std::string text);
        void clearText();

        /// @brief checks the current rect size and wraps the whole text
        void updateTextWrap();
        
        /// @brief removes the text at index 0
        void popFront() noexcept;

        const std::list<std::string>& getText() const;

        void draw();
        void update();

        Color color, textColor;

        void setPosition(float x, float y);
        void setSize(float width, float height);

        Font *font;

    private:
        std::list<std::string> text;
        unsigned int contentHeight;
        
        ScrollBar scrollBar; // it appears when text is higher than rect.height

        Rectangle rect;
    };
}