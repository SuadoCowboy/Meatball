#include "Utils.h"

void Meatball::checkHovered(bool &hovered, const Rectangle &rect, VoidFunc *onHover, VoidFunc *onRelease) {
    bool wasHovered = hovered;
    hovered = CheckCollisionPointRec(GetMousePosition(), rect);
    
    if (!wasHovered && hovered && onHover && *onHover)
        (*onHover)();
    
    if (hovered && onRelease && *onRelease && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        (*onRelease)();
}

Vector2 Meatball::getRectCenter(const Rectangle &rect) {
    return {rect.x+rect.width*0.5f, rect.y+rect.height*0.5f};
}

float Meatball::getRectCenterX(const Rectangle &rect) {
    return rect.x+rect.width*0.5;
}

float Meatball::getRectCenterY(const Rectangle &rect) {
    return rect.y+rect.height*0.5;
}

void Meatball::fitXYInRenderScreen(Rectangle &rect, const Vector2 &minPos, const Vector2 &maxPos) {
    if (rect.x < minPos.x) rect.x = minPos.x;
    else if (rect.x > GetRenderWidth()-maxPos.x) rect.x = GetRenderWidth()-maxPos.x;
    
    if (rect.y < minPos.y) rect.y = minPos.y;
    else if (rect.y > GetRenderHeight()-maxPos.y) rect.y = GetRenderHeight()-maxPos.y;
}

bool operator==(const Color &left, const Color &right) {
    return (left.r == right.r) && 
     (left.g == right.g) && 
     (left.b == right.b) && 
     (left.a == right.a);
}