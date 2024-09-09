#include "TextButton.h"

#include "Utils/Utils.h"
#include "Utils/DrawFuncs.h"
#include "MouseCursor.h"

Meatball::Config::TextButton Meatball::Defaults::textButtonConfig;

Meatball::TextButton::TextButton() {
    fontSize = config->font->baseSize;
}

Meatball::TextButton::TextButton(const Rectangle& rect)
 : rect(rect) {
    fontSize = config->font->baseSize;
 }

void Meatball::TextButton::update() {
    if (hovered)
        setCursor(MOUSE_CURSOR_POINTING_HAND, MouseCursorPriorityLevel::BUTTON);
    else
        resetCursor(MouseCursorPriorityLevel::BUTTON);

    checkHovered(hovered, rect, &onHover, &onRelease);
}

void Meatball::TextButton::drawText() {
    Meatball::drawText(*config->font, fontSize, text.c_str(), rect.x+rect.width*0.5f-measureTextWidth(*config->font, fontSize, text.c_str())*0.5f, rect.y+rect.height*0.5f, hovered? config->hoveredTextColor : config->textColor);
}