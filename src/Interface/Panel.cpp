#include "Panel.h"

Meatball::Panel::Panel(float x, float y, float width, float height)
    : rect{.x=x,.y=y,.width=width,.height=height}, color(BLACK) {}

Meatball::Panel::Panel(Rectangle rect) : rect(rect), color(BLACK) {}

void Meatball::Panel::draw() {
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, color);
}