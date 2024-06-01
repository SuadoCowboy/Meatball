#include "ScrollBar.h"

#include "Utils/Utils.h"

unsigned char Meatball::ScrollBar::scrollSpeed = 50;

static void setupScrollBarColors(Meatball::ScrollBar *scrollBar) {
    scrollBar->barColor = (Color){15,15,15,255};
    scrollBar->barHoveredColor = (Color){25,25,25,255};

    scrollBar->thumbColor = (Color){30,30,30,255};
    scrollBar->thumbHoveredColor1 = (Color){60,60,60,255};
    scrollBar->thumbHoveredColor2 = (Color){90,90,90,255};
}

Meatball::ScrollBar::ScrollBar() : visible(false), rect({0,0,0,0}),
 barHovered(false), thumbHovered(false), thumbY(0), thumbHeight(rect.height),
 scrollValue(0), dragging(false), dragOffsetY(0) {
    setupScrollBarColors(this);
}

Meatball::ScrollBar::ScrollBar(Rectangle rect, bool visible)
    : visible(visible), rect(rect), barHovered(false),
    thumbHovered(false), thumbY(0), thumbHeight(rect.height),
    scrollValue(0), dragging(false), dragOffsetY(0) {
        setupScrollBarColors(this);
    }

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

void Meatball::ScrollBar::draw() {
    if (!visible) return;

    DrawRectangle(rect.x, rect.y, rect.width, rect.height, barHovered? barHoveredColor : barColor);

    Color *actualScrollColor;
    if (barHovered) actualScrollColor = &thumbHoveredColor1;
    else actualScrollColor = &thumbColor;
    if (thumbHovered) actualScrollColor = &thumbHoveredColor2;

    DrawRectangle(rect.x, rect.y+thumbY, rect.width, thumbHeight, *actualScrollColor);
}

void Meatball::ScrollBar::update(const Rectangle& parentRect) {
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
        thumbY -= mouseWheelMove  *scrollSpeed  *(thumbHeight/rect.height);
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

const Rectangle& Meatball::ScrollBar::getRect() {
    return rect;
}

void Meatball::ScrollBar::updateThumbHeight(float viewHeight, float contentHeight) {
    if (contentHeight < viewHeight) contentHeight = viewHeight;
    
    thumbHeight = (viewHeight / contentHeight)  *rect.height;
    if (thumbHeight < 1) thumbHeight = 1;
}