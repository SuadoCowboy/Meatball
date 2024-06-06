#include "Button.h"

#include "Utils/Utils.h"

Meatball::Button::Button(Config::Button &config)
 : config(config), rect({0,0,0,0}) {}

Meatball::Button::Button(Rectangle &rect, Config::Button &config)
 : config(config), rect(rect) {}

void Meatball::Button::update() {
    checkHovered(hovered, rect, &onHover, &onRelease);
}

bool Meatball::Button::isHovered()
{
    return hovered;
}