#include "Button.h"

#include <sstream>

static inline void setupButton(Meatball::Button* button) {
    button->onClick = nullptr;
    button->onHover = nullptr;

    button->color = BLACK;
    button->hoveredColor = WHITE;

    button->textColor = WHITE;
    button->textHoveredColor = RAYWHITE;
}

Meatball::Button::Button() : rect{.x=0,.y=0,.width=0,.height=0} {
    setupButton(this);
}

Meatball::Button::Button(float x, float y, float width, float height) {
        rect = (Rectangle){x, y, width, height};
        setupButton(this);
    }

Meatball::Button::Button(Rectangle rect) : rect(rect) {
    setupButton(this);
}

void Meatball::Button::update() {
    bool wasHovered = hovered;
    hovered = CheckCollisionPointRec((Vector2){(float)GetMouseX(), (float)GetMouseY()}, rect);
    
    if (!wasHovered && hovered && onHover)
        (*onHover)();

    if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && onClick)
        (*onClick)();
}

const std::string& Meatball::Button::getText() {
    return text;
}

const unsigned char& Meatball::Button::getFontSize() {
    return fontSize;
}

void Meatball::Button::setText(std::string newText, unsigned char newFontSize) {
    if (newFontSize != 0) fontSize = newFontSize;

    int textWidth = MeasureText(newText.c_str(), fontSize);

    while (textWidth > rect.width) {
        newText = newText.substr(0, newText.size()-1);
        textWidth = MeasureText(newText.c_str(), fontSize);
    }

    text = newText;
}

void Meatball::Button::drawText()
{   
    DrawText(text.c_str(), rect.x, rect.y, fontSize, hovered? textHoveredColor : textColor);
}

void Meatball::Button::drawTextCentered(bool centerX, bool centerY) {
    int textWidthHalf = MeasureText(text.c_str(), fontSize)/2;

    DrawText(text.c_str(), centerX? rect.x+rect.width/2-textWidthHalf : rect.x, centerY? rect.y+rect.height/2-fontSize/2 : rect.y, fontSize, hovered? textHoveredColor : textColor);
}

void Meatball::Button::drawRect() {
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, hovered? hoveredColor : color);
}

void Meatball::Button::drawX() {
    /*
    .
     .
      .
    */
    DrawLine(rect.x,rect.y, rect.x+rect.width,rect.y+rect.height, hovered? hoveredColor : color);
    /*
      .
     .
    .
    */
    DrawLine(rect.x,rect.y+rect.height, rect.x+rect.width,rect.y, hovered? hoveredColor : color);
}

bool Meatball::Button::isHovered()
{
    return hovered;
}