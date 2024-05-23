#include "ScrollBar.h"

#include "Utils/Utils.h"

unsigned char Meatball::ScrollBar::scrollSpeed = 4;

Meatball::ScrollBar::ScrollBar(Rectangle barRect, bool visible)
    : barRect(barRect), visible(visible),
    barHovered(false), scrollHovered(false), scrollY(0) {
        barColor = (Color){15,15,15,255};
        barHoveredColor = (Color){25,25,25,255};

        scrollColor = (Color){30,30,30,255};
        scrollHoveredColor1 = (Color){60,60,60,255};
        scrollHoveredColor2 = (Color){90,90,90,255};
    }

float Meatball::ScrollBar::getScrollHeight() const {
    return barRect.height/10; // Adjust scrollbar height proportionally
}

short Meatball::ScrollBar::getScrollY() const {
    return scrollY;
}

void Meatball::ScrollBar::draw() {
    if (!visible) return;

    DrawRectangle(barRect.x, barRect.y, barRect.width, barRect.height, barHovered? barHoveredColor : barColor);

    Color* actualScrollColor;
    if (barHovered) actualScrollColor = &scrollHoveredColor1;
    else actualScrollColor = &scrollColor;
    if (scrollHovered) actualScrollColor = &scrollHoveredColor2;

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
            scrollY = mousePosition.y-barRect.y-scrollHeight/2;
        else if (GetMouseWheelMove() != 0)
            scrollY -= GetMouseWheelMove()*scrollSpeed;
        else return;

        // if up limit
        if (scrollY < 0) scrollY = 0;
        // if down limit
        if (scrollY+scrollHeight >= barRect.height) scrollY = barRect.height-scrollHeight;
    } 
}