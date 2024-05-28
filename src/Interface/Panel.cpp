#include "Panel.h"

Meatball::Panel::Panel() : color(BLACK) {
    rect = {0,0,0,0};
}

Meatball::Panel::Panel(float x, float y, float width, float height) : color(BLACK) {
    rect = {x, y, width, height};
}

Meatball::Panel::Panel(Rectangle rect) : rect(rect), color(BLACK) {}

void Meatball::Panel::draw() {
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, color);
}