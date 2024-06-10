#include "ScrollBar.h"

#include "Utils/Utils.h"

unsigned char Meatball::ScrollBar::scrollSpeed = 50;

std::shared_ptr<Meatball::Config::ScrollBar> Meatball::Defaults::scrollBarConfig;

Meatball::Config::ScrollBar::ScrollBar()
 : barColor({15,15,15,255}), barHoveredColor({25,25,25,255}),
 thumbColor({30,30,30,255}), thumbHoveredColor1({60,60,60,255}), thumbHoveredColor2({90,90,90,255}) {}

Meatball::ScrollBar::ScrollBar()
 : visible(false), rect({0,0,0,0}), thumbHeight(rect.height) {}

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

const float& Meatball::ScrollBar::getScrollValue() const {
    return scrollValue;
}

void Meatball::ScrollBar::setScrollValue(float value) {
    scrollValue = value;
}

void Meatball::ScrollBar::draw() {
    if (!visible) return;

    DrawRectangle(rect.x, rect.y, rect.width, rect.height, barHovered? config->barHoveredColor : config->barColor);

    Color *actualScrollColor;
    if (barHovered) actualScrollColor = &config->thumbHoveredColor1;
    else actualScrollColor = &config->thumbColor;
    if (thumbHovered) actualScrollColor = &config->thumbHoveredColor2;

    DrawRectangle(rect.x, rect.y+thumbY, rect.width, thumbHeight, *actualScrollColor);
}

void Meatball::ScrollBar::update(const Rectangle &parentRect) {
    Vector2 mousePosition = GetMousePosition();
    
    barHovered = CheckCollisionPointRec(mousePosition, rect);
    
    thumbHovered = CheckCollisionPointRec(mousePosition,
        {rect.x, rect.y+thumbY, rect.width, thumbHeight});

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (thumbHovered) {
            dragging = true;
            dragOffsetY = thumbY - mousePosition.y;
        } else if (barHovered)
            thumbY = mousePosition.y - thumbHeight/2 - rect.y;
        
    } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) dragging = false;
    
    float mouseWheelMove = GetMouseWheelMove();
    if (mouseWheelMove != 0 && CheckCollisionPointRec(GetMousePosition(), parentRect)) {
        thumbY -= mouseWheelMove * scrollSpeed * (thumbHeight/rect.height);
    }
    
    if (dragging) {
        thumbY = mousePosition.y+dragOffsetY;
    }
    
    // if up limit
    if (thumbY < 0) thumbY = 0;
    // if down limit
    if (thumbY+thumbHeight > rect.height) thumbY = rect.height-thumbHeight;
    
    scrollValue = thumbY / thumbHeight;
}

const Rectangle &Meatball::ScrollBar::getRect() {
    return rect;
}

void Meatball::ScrollBar::updateThumbHeight(float viewHeight, float contentHeight) {
    if (contentHeight < viewHeight) contentHeight = viewHeight;
    
    float newThumbHeight = (viewHeight / contentHeight) * rect.height;
    if (newThumbHeight < 1) newThumbHeight = 1;

    if (thumbY+thumbHeight == rect.height) thumbY = rect.height-newThumbHeight;
    else thumbY = scrollValue*newThumbHeight;

    thumbHeight = newThumbHeight;
}