#include "Button.h"

#include "Utils/Utils.h"

Meatball::Button::Button(std::shared_ptr<Config::Button> config)
 : config(config), rect({0,0,0,0}) {}

Meatball::Button::Button(Rectangle &rect, std::shared_ptr<Config::Button> config)
 : config(config), rect(rect) {}

void Meatball::Button::update() {
    checkHovered(hovered, rect, &onHover, &onRelease);
}

bool Meatball::Button::isHovered()
{
    return hovered;
}