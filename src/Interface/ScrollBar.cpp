#include "ScrollBar.h"

#include "Utils/Utils.h"
#include "MouseCursor.h"

unsigned char Meatball::ScrollBar::scrollSpeed = 50;

Meatball::ScrollBar::ScrollBar(const Rectangle& rect, bool visible)
 : visible(visible), rect(rect), thumbHeight(rect.height) {}

void Meatball::ScrollBar::setPosition(float x, float y) {
    rect.x = x;
    rect.y = y;
}

void Meatball::ScrollBar::setSize(float width, float height) {
    if (rect.height != 0)
        thumbY *= height/rect.height;
    
    rect.width = width;
    rect.height = height;
}

float Meatball::ScrollBar::getScrollValue() const {
    return scrollValue;
}

void Meatball::ScrollBar::setScrollValue(float value) {
    scrollValue = value;
}

void Meatball::ScrollBar::draw(
        const Color& barColor,
        const Color& barHoveredColor,
        const Color& thumbColor,
        const Color& thumbHoveredColor1,
        const Color& thumbHoveredColor2
) const {
    if (!visible) return;

    DrawRectangle(rect.x, rect.y, rect.width, rect.height, barHovered? barHoveredColor : barColor);

    Color actualScrollColor;
    if (barHovered || dragging) actualScrollColor = thumbHoveredColor1;
    else actualScrollColor = thumbColor;
    if (thumbHovered) actualScrollColor = thumbHoveredColor2;

    DrawRectangle(rect.x, rect.y+thumbY, rect.width, thumbHeight, actualScrollColor);
}

void Meatball::ScrollBar::onMousePress(int button) {
    if (button == MOUSE_BUTTON_LEFT) {
        if (thumbHovered) {
            dragging = true;
            dragOffsetY = thumbY - GetMouseY();
        } else if (barHovered)
            thumbY = GetMouseY() - thumbHeight/2 - rect.y;
        
    }
}

void Meatball::ScrollBar::onMouseRelease(int button) {
    if (button == MOUSE_BUTTON_LEFT)
        dragging = false;
}

void Meatball::ScrollBar::onMouseWheel(const Vector2& dir, const Rectangle& parentRect) {
    if (dir.y != 0 && CheckCollisionPointRec(GetMousePosition(), parentRect))
        thumbY -= dir.y * scrollSpeed * (thumbHeight/rect.height);
    
    fitThumb();
}

void Meatball::ScrollBar::fitThumb() {
    // if up limit
    if (thumbY < 0) thumbY = 0;
    // if down limit
    if (thumbY+thumbHeight > rect.height) thumbY = rect.height-thumbHeight;
    
    scrollValue = thumbY / thumbHeight;
}

void Meatball::ScrollBar::onMouseMove(const Vector2& mousePosition) {
    barHovered = CheckCollisionPointRec(mousePosition, rect);

    if (barHovered)
        setCursor(MOUSE_CURSOR_POINTING_HAND, MouseCursorPriorityLevel::SCROLL_BAR);
    else
        resetCursor(MouseCursorPriorityLevel::SCROLL_BAR);
    
    thumbHovered = CheckCollisionPointRec(mousePosition,
        {rect.x, rect.y+thumbY, rect.width, thumbHeight});
    
    if (dragging)
        thumbY = mousePosition.y+dragOffsetY;
    
    fitThumb();
}

const Rectangle& Meatball::ScrollBar::getRect() const {
    return rect;
}

void Meatball::ScrollBar::updateThumbHeight(float viewHeight, float contentHeight) {
    if (contentHeight < viewHeight) contentHeight = viewHeight;
    
    float newThumbHeight = (viewHeight / contentHeight) * rect.height;
    if (newThumbHeight < 1) newThumbHeight = 1;

    if (thumbY+thumbHeight <= rect.height+1 && thumbY+thumbHeight >= rect.height-1) thumbY = rect.height-newThumbHeight;
    else thumbY = scrollValue*newThumbHeight;

    thumbHeight = newThumbHeight;
}