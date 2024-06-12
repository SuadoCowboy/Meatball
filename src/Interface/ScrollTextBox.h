#pragma once

#include <string>
#include <list>
#include <memory>

#include <raylib.h>

#include "ScrollBar.h"

namespace Meatball {
    namespace Config {
        struct ScrollTextBox {
            ScrollTextBox();

            std::shared_ptr<Font> font;
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

        const Rectangle &getRect() const;
        const unsigned int &getContentHeight() const;
        ScrollBar &getScrollBar();

        void appendText(std::string text, const std::shared_ptr<Color>& color);
        void clearText();

        /// @brief checks the current rect size and wraps the whole text
        void updateTextWrap();
        
        /// @brief removes the text at index 0
        void popFront() noexcept;

        const std::list<std::pair<std::string, const std::shared_ptr<Color>>> &getText() const;

        void draw();
        void drawScrollbar();
        void update();

        void setPosition(float x, float y);
        void setSize(float width, float height);

        std::shared_ptr<Config::ScrollTextBox> config;

    private:
        std::list<std::pair<std::string, const std::shared_ptr<Color>>> text;
        unsigned int contentHeight = 0;
        
        ScrollBar scrollBar; // it appears when text is higher than rect.height
        Rectangle rect;
    };
}