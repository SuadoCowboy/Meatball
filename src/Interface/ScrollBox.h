#pragma once

#include <string>
#include <deque>

#include <raylib.h>

#include "ScrollBar.h"
#include "../Utils/Utils.h"

namespace Meatball {
    /// @brief basically, a rect being used to draw text but with scrolling
    /// @note it also have a scrollable bar when text is higher than rect.height
    /// @note if you're looking for text input see TextInputBox instead
    class ScrollBox {
    public:
        ScrollBox();
        ScrollBox(const Rectangle& rect, unsigned short fontSize);

        const Rectangle& getRect() const;
        unsigned int getContentHeight() const;

        void appendText(const std::string& text, const Font& font, const Color& color);
        void clearText();

        /// @brief checks the current rect size and wraps the whole text
        void updateTextWrap(const Font& font);
        
        /// @brief removes the text at index 0
        void popFront() noexcept;

        const std::deque<ColoredText>& getText() const;

        void draw(const Color& color, const Font& font) const;
        void update();

        void setPosition(float x, float y);
        void setSize(float width, float height);

        ScrollBar scrollBar;

        unsigned short fontSize = 10;
    private:
        std::deque<ColoredText> text;
        unsigned int contentHeight = 0;
        
        Rectangle rect;
    };
}