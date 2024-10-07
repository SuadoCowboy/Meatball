#include "Button.h"

#include "Utils/Utils.h"
#include "MouseCursor.h"

Meatball::Button::Button(const Rectangle& rect)
 : rect(rect) {}

void Meatball::Button::onMouseMove(const Vector2& mousePosition) {
    hovered = handleRectHover(hovered, mousePosition, rect, onHover, onUnhover);

    if (hovered)
        setCursor(MOUSE_CURSOR_POINTING_HAND, MouseCursorPriorityLevel::BUTTON);
    else
        resetCursor(MouseCursorPriorityLevel::BUTTON);
}