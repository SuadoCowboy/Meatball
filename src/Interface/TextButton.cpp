#include "TextButton.h"

#include "Utils/Utils.h"
#include "Utils/DrawFuncs.h"
#include "MouseCursor.h"

Meatball::TextButton::TextButton(const Rectangle& rect, unsigned short fontSize)
 : rect(rect), fontSize(fontSize) {}

void Meatball::TextButton::update() {
    if (hovered)
        setCursor(MOUSE_CURSOR_POINTING_HAND, MouseCursorPriorityLevel::BUTTON);
    else
        resetCursor(MouseCursorPriorityLevel::BUTTON);

    checkHovered(hovered, rect, &onHover, &onRelease);
}

void Meatball::TextButton::drawText(const Font& font, const Color& textColor, const Color& hoveredTextColor) const {
    Meatball::drawText(font, fontSize, text.c_str(), rect.x+rect.width*0.5f-measureTextWidth(font, fontSize, text.c_str())*0.5f, rect.y+rect.height*0.5f, hovered? hoveredTextColor : textColor);
}