#include "Button.h"

Meatball::Button::Button(float x, float y, float width, float height)
    : rect{.x=x, .y=y, .width=width, .height=height} {}

Meatball::Button::Button(Rectangle rect) : rect(rect) {}

void Meatball::Button::update(VoidFunc onClick, VoidFunc onHover) {
    bool wasHovered = hovered;
    hovered = CheckCollisionPointRec((Vector2){GetMouseX(), GetMouseY()}, rect);
    
    if (!wasHovered && hovered)
        onHover();

    if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        onClick();
}

void Meatball::Button::draw(std::string text, int fontSize)
{
    int textWidth = MeasureText(text.c_str(), fontSize);

    int offset = 1;
    while (textWidth > rect.width)
        textWidth = MeasureText(text.substr(0, text.size()-offset).c_str(), fontSize);

    DrawRectangle(rect.x, rect.y, rect.width, rect.height, hovered? RED : BLACK);

    DrawText(text.c_str(), rect.x, rect.y, fontSize, hovered? RED : BLACK);
}

bool Meatball::Button::isHovered()
{
    return hovered;
}