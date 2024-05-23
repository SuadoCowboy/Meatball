#include "Utils.h"

void Meatball::checkHovered(bool& hovered, const Rectangle& rect, VoidFunc* onHover, VoidFunc* onClick, VoidFunc* onRelease) {
    bool wasHovered = hovered;
    hovered = CheckCollisionPointRec(GetMousePosition(), rect);
    
    if (!wasHovered && hovered && onHover)
        (*onHover)();

    if (hovered && onClick && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        (*onClick)();
    
    if (onRelease && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        (*onRelease)();
}