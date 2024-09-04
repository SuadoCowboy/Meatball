#include "TextButton.h"

#include "Utils/Utils.h"
#include "Utils/DrawFuncs.h"
#include "MouseCursor.h"

Meatball::Config::TextButton Meatball::Defaults::textButtonConfig;

Meatball::TextButton::TextButton(const Rectangle& rect)
 : rect(rect) {}

bool Meatball::TextButton::isHovered() {
    return hovered;
}

void Meatball::TextButton::update() {
    if (hovered)
        setCursor(MOUSE_CURSOR_POINTING_HAND, MouseCursorPriorityLevel::BUTTON);
    else
        resetCursor(MouseCursorPriorityLevel::BUTTON);

    checkHovered(hovered, rect, &onHover, &onRelease);
}

void Meatball::TextButton::drawText() {
    BeginScissorMode(rect.x, rect.y, rect.width, rect.height);
    
    Meatball::drawText(*config->font, config->fontSize, text.c_str(), rect.x+rect.width*0.5, rect.y+rect.height*0.5, hovered? config->hoveredTextColor : config->textColor);

    EndScissorMode();
}