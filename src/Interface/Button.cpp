#include "Button.h"

#include "Utils/Utils.h"
#include "MouseCursor.h"

Meatball::Config::Button Meatball::Defaults::buttonConfig;

Meatball::Config::Button::Button()
 : color(BLACK), hoveredColor(WHITE) {}

Meatball::Button::Button()
 : rect({0,0,0,0}) {}

Meatball::Button::Button(const Rectangle& rect)
 : rect(rect) {}

void Meatball::Button::update() {
    if (hovered)
        setCursor(MOUSE_CURSOR_POINTING_HAND, MouseCursorPriorityLevel::BUTTON);
    else
        resetCursor(MouseCursorPriorityLevel::BUTTON);
    
    checkHovered(hovered, rect, &onHover, &onRelease);
}

bool Meatball::Button::isHovered()
{
    return hovered;
}