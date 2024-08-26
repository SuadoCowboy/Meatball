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

            Font* font;
            unsigned short fontSize;

            Color color;
        };
    }

    struct ColoredText {
        std::string text;
        Color color;

        ColoredText(const std::string& text, const Color& color) : text(text), color(color) {}
    };

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

        void appendText(std::string text, Color& color);
        void clearText();

        /// @brief checks the current rect size and wraps the whole text
        void updateTextWrap();
        
        /// @brief removes the text at index 0
        void popFront() noexcept;

        const std::list<ColoredText> &getText() const;

        void draw();
        void drawScrollbar();
        void update();

        void setPosition(float x, float y);
        void setSize(float width, float height);

        std::shared_ptr<Config::ScrollTextBox> config;

    private:
        std::list<ColoredText> text;
        unsigned int contentHeight = 0;
        
        ScrollBar scrollBar; // it appears when text is higher than rect.height
        Rectangle rect;
    };
}