#include "TextButton.h"

#include "Button.h"

#include "Utils/Utils.h"
#include "FontsHandler.h"

using fh = Meatball::FontsHandler;

Meatball::TextButton::TextButton(Config::TextButton &config)
 : config(config), rect({0,0,0,0}) {}

Meatball::TextButton::TextButton(Rectangle rect, Config::TextButton &config)
 : config(config), rect(rect) {}

bool Meatball::TextButton::isHovered() {
    return hovered;
}

void Meatball::TextButton::update() {
    checkHovered(hovered, rect, &onHover, &onRelease);
}

const std::string &Meatball::TextButton::getText() {
    return text;
}

void Meatball::TextButton::setText(std::string &newText) {
    int textWidth = fh::MeasureTextWidth(config.font, newText.c_str());

    while (textWidth > rect.width) {
        newText = newText.substr(0, newText.size()-1);
        textWidth = fh::MeasureTextWidth(config.font, newText.c_str());
    }

    text = newText;
}