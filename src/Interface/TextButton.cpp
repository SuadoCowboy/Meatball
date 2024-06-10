#include "TextButton.h"

#include "Utils/Utils.h"
#include "Utils/DrawFuncs.h"
#include "FontsHandler.h"

using fh = Meatball::FontsHandler;

std::shared_ptr<Meatball::Config::TextButton> Meatball::Defaults::textButtonConfig;

Meatball::Config::TextButton::TextButton()
 : color(BLACK), textColor(WHITE), hoveredColor(WHITE), hoveredTextColor(BLACK) {
    font = FontsHandler::get("default");
}

Meatball::TextButton::TextButton()
 : rect({0,0,0,0}) {}

Meatball::TextButton::TextButton(Rectangle rect)
 : rect(rect) {}

bool Meatball::TextButton::isHovered() {
    return hovered;
}

void Meatball::TextButton::update() {
    checkHovered(hovered, rect, &onHover, &onRelease);
}

void Meatball::TextButton::drawText() {
    BeginScissorMode(rect.x, rect.y, rect.width, rect.height);
    
    Meatball::drawText(config->font, text.c_str(), rect.x+rect.width*0.5, rect.y+rect.height*0.5, hovered? config->hoveredTextColor : config->textColor);

    EndScissorMode();
}

const std::string &Meatball::TextButton::getText() {
    return text;
}

void Meatball::TextButton::setText(const std::string &_newText) {
    text = _newText;

    int textWidth = fh::MeasureTextWidth(config->font, text.c_str());

    while (textWidth > rect.width) {
        text.erase(text.end());
        textWidth = fh::MeasureTextWidth(config->font, text.c_str());
    }
}