#include "Button.h"

#include <sstream>

Meatball::Button::Button(float x, float y, float width, float height)
    : rect{.x=x, .y=y, .width=width, .height=height}, color(BLACK) {}

Meatball::Button::Button(Rectangle rect) : rect(rect), color(BLACK) {}

void Meatball::Button::update() {
    bool wasHovered = hovered;
    hovered = CheckCollisionPointRec((Vector2){(float)GetMouseX(), (float)GetMouseY()}, rect);
    
    if (!wasHovered && hovered)
        onHover();

    if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        onClick();
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

    DrawText(text.c_str(), centerX? rect.x/2-textWidthHalf : rect.x, centerY? rect.y/2-fontSize/2 : rect.y, fontSize, hovered? textHoveredColor : textColor);
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
    DrawLine(0,0, rect.x,rect.y, hovered? hoveredColor : color);
    /*
      .
     .
    .
    */
    DrawLine(0,rect.y, rect.x,0, hovered? hoveredColor : color);
}

bool Meatball::Button::isHovered()
{
    return hovered;
}