#include "ScrollBar.h"

#include "Utils/Utils.h"

unsigned char Meatball::ScrollBar::scrollSpeed = 4;

Meatball::ScrollBar::ScrollBar(Rectangle barRect, bool visible)
    : barRect(barRect), barColor(WHITE), barHoveredColor(WHITE), scrollColor(RAYWHITE),
    scrollHoveredColor1(GRAY), scrollHoveredColor2(DARKGRAY), visible(visible),
    barHovered(false), scrollHovered(false), scrollY(0) {}

float Meatball::ScrollBar::getScrollHeight() const {
    return barRect.height/10; // Adjust scrollbar height proportionally
}

unsigned short Meatball::ScrollBar::getScrollY() const {
    return scrollY;
}

void Meatball::ScrollBar::draw() {
    if (!visible) return;

    DrawRectangle(barRect.x, barRect.y, barRect.width, barRect.height, barHovered? barHoveredColor : barColor);

    Color* actualScrollColor;
    if (scrollHovered) actualScrollColor = &scrollHoveredColor2;
    else if (barHovered) actualScrollColor = &scrollHoveredColor1;
    else actualScrollColor = &scrollColor;

    DrawRectangle(barRect.x, barRect.y+scrollY, barRect.width, getScrollHeight(), *actualScrollColor);
}

void Meatball::ScrollBar::update() {
    if (!visible) return;

    Vector2 mousePosition = GetMousePosition();
    
    barHovered = CheckCollisionPointRec(mousePosition, barRect);
    
    float scrollHeight = getScrollHeight();
    scrollHovered = CheckCollisionPointRec(mousePosition,
        (Rectangle){barRect.x, (float)scrollY, barRect.width, scrollHeight});

    if (barHovered) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
            scrollY = mousePosition.y-barRect.y;
        else if (GetMouseWheelMove() != 0)
            scrollY = GetMouseWheelMove()*scrollSpeed-barRect.y;
        else return;

        // if up limit
        if (scrollY-scrollHeight/2 > 0) scrollY -= scrollHeight/2;
        // if down limit
        if (scrollY+scrollHeight >= barRect.height) scrollY = barRect.height-scrollHeight;
    } 
}