#include "ScrollBar.h"

#include "Utils/Utils.h"

unsigned char Meatball::ScrollBar::scrollSpeed = 10;

static inline void updateScrollValue(float& scrollValue, const float& thumbY, const float& thumbHeight, const Rectangle& barRect) {
    scrollValue = thumbY / thumbHeight;
}

Meatball::ScrollBar::ScrollBar(Rectangle barRect, bool visible)
    : barRect(barRect), visible(visible),
    barHovered(false), thumbHovered(false), thumbY(0), thumbHeight(barRect.height), dragging(false), dragOffsetY(0) {
        barColor = (Color){15,15,15,255};
        barHoveredColor = (Color){25,25,25,255};

        thumbColor = (Color){30,30,30,255};
        thumbHoveredColor1 = (Color){60,60,60,255};
        thumbHoveredColor2 = (Color){90,90,90,255};
    }

float Meatball::ScrollBar::getScrollValue() const {
    return scrollValue;
}

void Meatball::ScrollBar::draw() {
    if (!visible) return;

    DrawRectangle(barRect.x, barRect.y, barRect.width, barRect.height, barHovered? barHoveredColor : barColor);

    Color* actualScrollColor;
    if (barHovered) actualScrollColor = &thumbHoveredColor1;
    else actualScrollColor = &thumbColor;
    if (thumbHovered) actualScrollColor = &thumbHoveredColor2;

    DrawRectangle(barRect.x, barRect.y+thumbY, barRect.width, thumbHeight, *actualScrollColor);
}

void Meatball::ScrollBar::update(const Rectangle& parentRect) {
    if (!visible) return;

    Vector2 mousePosition = GetMousePosition();
    
    barHovered = CheckCollisionPointRec(mousePosition, barRect);
    
    thumbHovered = CheckCollisionPointRec(mousePosition,
        (Rectangle){barRect.x, barRect.y+thumbY, barRect.width, thumbHeight});

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (thumbHovered) {
            dragging = true;
            dragOffsetY = thumbY+barRect.y - mousePosition.y;
        } else if (barHovered)
            thumbY = mousePosition.y - thumbHeight/2 - barRect.y;
        
    } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) dragging = false;
    
    float mouseWheelMove = GetMouseWheelMove();
    if (mouseWheelMove != 0 && CheckCollisionPointRec(GetMousePosition(), parentRect)) {
        thumbY -= mouseWheelMove * scrollSpeed * (thumbHeight/barRect.height);
    }
    
    if (dragging) {
        thumbY = mousePosition.y-barRect.y+dragOffsetY;
    }
    
    // if up limit
    if (thumbY < 0) thumbY = 0;
    // if down limit
    if (thumbY+thumbHeight > barRect.height) thumbY = barRect.height-thumbHeight;
    
    updateScrollValue(scrollValue, thumbY, thumbHeight, barRect);
}

void Meatball::ScrollBar::updateThumbHeight(float viewHeight, float contentHeight) {
    thumbHeight = (viewHeight / contentHeight) * barRect.height;
    if (thumbHeight < 1) thumbHeight = 1;
}