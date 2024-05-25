#pragma once

#include <string>
#include <list>

#include <raylib.h>

#include "ScrollBar.h"

namespace Meatball {
    /// @brief basically, a rect being used to draw text
    /// @note it also have a scrollable bar when text is higher than rect.height
    /// @note if you're looking for text input see TextInputBox instead
    class TextBox {
    public:
        TextBox();
        TextBox(Rectangle rect, unsigned char fontSize);

        void setFontSize(unsigned char newFontSize);

        const unsigned char& getFontSize() const;
        const Rectangle& getRect() const;
        const unsigned int getContentHeight() const;
        ScrollBar& getScrollBar();

        void appendText(std::string text);
        void clearText();
        
        /// @brief removes the text at index 0
        void popFront() noexcept;

        const std::list<std::string>& getText() const;

        void draw();
        void update();

        Color color, textColor;

        void setPosition(float x, float y);
        void setSize(float width, float height);

        // TODO: Events that can be used to drag and resize rect.
        // See appendText function, there is a good thing that
        // should be used in the case of resizing the rect.
        //VoidFunc onClick, onRelease;

    private:
        std::list<std::string> text;
        unsigned char fontSize;
        unsigned int contentHeight;
        
        ScrollBar scrollBar; // it appears when text is higher than rect.height

        Rectangle rect;
    };
}