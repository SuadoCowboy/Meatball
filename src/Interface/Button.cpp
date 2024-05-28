#include "Button.h"

#include "Utils/Utils.h"
#include "FontsHandler.h"

using fh = Meatball::FontsHandler;

static inline void setupButton(Meatball::Button* button) {
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
        rect = {x, y, width, height};
        setupButton(this);
    }

Meatball::Button::Button(Rectangle rect) : rect(rect) {
    setupButton(this);
}

void Meatball::Button::update() {
    checkHovered(hovered, rect, &onHover, &onRelease);
}

const std::string& Meatball::Button::getText() {
    return text;
}

void Meatball::Button::setText(std::string newText) {
    int textWidth = fh::MeasureTextWidth(font, newText.c_str());

    while (textWidth > rect.width) {
        newText = newText.substr(0, newText.size()-1);
        textWidth = fh::MeasureTextWidth(font, newText.c_str());
    }

    text = newText;
}

void Meatball::Button::drawText()
{   
    fh::DrawText(font,text.c_str(), rect.x, rect.y, hovered? textHoveredColor : textColor);
}

void Meatball::Button::drawTextCentered(bool centerX, bool centerY) {
    int textWidthHalf = fh::MeasureTextWidth(font, text.c_str())/2;

    fh::DrawText(font, text.c_str(), centerX? rect.x+rect.width/2-textWidthHalf : rect.x, centerY? rect.y+rect.height/2-(float)font->baseSize/2 : rect.y, hovered? textHoveredColor : textColor);
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