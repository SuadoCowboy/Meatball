#pragma once

#include <string>
#include <deque>

#include <raylib.h>

#include "ScrollBar.h"
#include "../Utils/Utils.h"

namespace Meatball {
    namespace Config {
        struct ScrollTextBox {
            ScrollTextBox();

            Font* font;
            unsigned short fontSize;

            Color color;
        };
    }

    namespace Defaults {
        extern Config::ScrollTextBox scrollTextBoxConfig;
    }
    
    /// @brief basically, a rect being used to draw text but with scrolling
    /// @note it also have a scrollable bar when text is higher than rect.height
    /// @note if you're looking for text input see TextInputBox instead
    class ScrollTextBox {
    public:
        ScrollTextBox();
        ScrollTextBox(const Rectangle& rect);

        const Rectangle& getRect() const;
        unsigned int getContentHeight() const;
        ScrollBar &getScrollBar();

        void appendText(const std::string& text, const Color& color);
        void clearText();

        /// @brief checks the current rect size and wraps the whole text
        void updateTextWrap();
        
        /// @brief removes the text at index 0
        void popFront() noexcept;

        const std::deque<ColoredText> &getText() const;

        void draw();
        void drawScrollbar();
        void update();

        void setPosition(float x, float y);
        void setSize(float width, float height);

        Config::ScrollTextBox *config = &Defaults::scrollTextBoxConfig;

    private:
        std::deque<ColoredText> text;
        unsigned int contentHeight = 0;
        
        ScrollBar scrollBar; // it appears when text is higher than rect.height
        Rectangle rect;
    };
}