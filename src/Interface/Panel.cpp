#include "Panel.h"

Meatball::Panel::Panel(float x, float y, float width, float height)
    : rect{.x=x,.y=y,.width=width,.height=height} {}

Meatball::Panel::Panel(Rectangle rect) : rect(rect) {}

void Meatball::Panel::update(VoidFunc onClick, VoidFunc onHover) {
    
}

void Meatball::Panel::draw() {
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, color);
}