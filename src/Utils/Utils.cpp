#include "Utils.h"

void Meatball::checkHovered(bool& hovered, const Rectangle& rect, VoidFunc* onHover, VoidFunc* onClick, VoidFunc* onRelease) {
    bool wasHovered = hovered;
    hovered = CheckCollisionPointRec(GetMousePosition(), rect);
    
    if (!wasHovered && hovered && onHover && *onHover)
        (*onHover)();

    if (hovered && onClick && *onClick && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        (*onClick)();
    
    if (hovered && onRelease && *onRelease && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        (*onRelease)();
}

float Meatball::getContentHeight(float viewHeight, float fontSize, const std::list<std::string>& text) {
    size_t lineIdx = 0;
    for (auto& line : text) {
        size_t newLineIdx = 0;
        while (newLineIdx != std::string::npos) {
            newLineIdx = line.find('\n', newLineIdx+1);
            lineIdx++;
        }
    }

    float contentHeight = lineIdx*fontSize;
    if (contentHeight < viewHeight) contentHeight = viewHeight;

    return contentHeight;
}