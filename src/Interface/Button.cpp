#include "Button.h"

#include "Utils/Utils.h"

std::shared_ptr<Meatball::Config::Button> Meatball::Defaults::buttonConfig;

Meatball::Config::Button::Button()
 : color(BLACK), hoveredColor(WHITE) {}

Meatball::Button::Button()
 : rect({0,0,0,0}) {}

Meatball::Button::Button(Rectangle &rect)
 : rect(rect) {}

void Meatball::Button::update() {
    checkHovered(hovered, rect, &onHover, &onRelease);
}

bool Meatball::Button::isHovered()
{
    return hovered;
}