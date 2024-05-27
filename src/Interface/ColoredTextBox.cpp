#include "ColoredTextBox.h"

#include "FontsHandler.h"

using fh = Meatball::FontsHandler;

Meatball::ColoredTextBox::ColoredTextBox() : backgroundColor(BLACK) {
    rect = {0,0,0,0};
    font = FontsHandler::get("default");
}

Meatball::ColoredTextBox::ColoredTextBox(float x, float y, float width, float height, Font* font)
    : font(font), backgroundColor(BLACK) {
        rect = {x, y, width, height};
}

void Meatball::ColoredTextBox::pushText(const std::string& text, Color color) {
    coloredText.push_back(std::pair<std::string, Color>(text, color));
}

void Meatball::ColoredTextBox::draw() {
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, backgroundColor);
    BeginScissorMode(rect.x, rect.y, rect.width, rect.height);

    float x = rect.x;
    for (auto& pair : coloredText) {
        fh::DrawText(font, pair.first.c_str(), x, rect.y+1, pair.second);
        x += fh::MeasureTextWidth(font, pair.first.c_str())+1;
    }

    EndScissorMode();
}