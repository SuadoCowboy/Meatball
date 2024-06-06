#include "Utils.h"

void Meatball::checkHovered(bool &hovered, const Rectangle &rect, VoidFunc *onHover, VoidFunc *onRelease) {
    bool wasHovered = hovered;
    hovered = CheckCollisionPointRec(GetMousePosition(), rect);
    
    if (!wasHovered && hovered && onHover && *onHover)
        (*onHover)();
    
    if (hovered && onRelease && *onRelease && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        (*onRelease)();
}

float Meatball::getContentHeight(float fontSize, const std::list<std::string> &text) {
    size_t lineIdx = 0;
    for (auto &line : text) {
        size_t newLineIdx = 0;
        while (newLineIdx != std::string::npos) {
            newLineIdx = line.find('\n', newLineIdx+1);
            lineIdx++;
        }
    }

    return lineIdx*fontSize; // contentHeight
}

Vector2 Meatball::getRectCenter(const Rectangle &rect) {
    return {rect.x+rect.width*0.5, rect.y+rect.height*0.5};
}

float Meatball::getRectCenterX(const Rectangle &rect) {
    return rect.x+rect.width*0.5;
}

float Meatball::getRectCenterY(const Rectangle &rect) {
    return rect.y+rect.height*0.5;
}

bool operator==(const Color &left, const Color &right) {
    return (left.r == right.r) && 
     (left.g == right.g) && 
     (left.b == right.b) && 
     (left.a == right.a);
}