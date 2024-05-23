#include "TextBox.h"

#include "Utils/Utils.h"

Meatball::TextBox::TextBox()
    : rect((Rectangle){0,0,0,0}), fontSize(1), color(BLACK),
    textColor(WHITE), scrollBar((Rectangle){0,0,0,0}) {}

Meatball::TextBox::TextBox(Rectangle rect, unsigned char fontSize)
    : rect(rect), fontSize(fontSize), color(BLACK), textColor(WHITE),
    scrollBar((Rectangle){rect.x,rect.y+rect.width, 0, rect.height}) {}

void Meatball::TextBox::appendText(const std::string& newText) {
    if (MeasureText(newText.c_str(), fontSize) < rect.width-scrollBar.barRect.width) {
        text.push_back(newText);
        return;
    }

    auto it = text.end();
    std::string newTextCopy = newText;
    size_t columnIdx = newTextCopy.size()-1;
    while (MeasureText(newTextCopy.c_str(), fontSize) >= rect.width-scrollBar.barRect.width) {
        newTextCopy = newText.substr(0, columnIdx);
        columnIdx--;
    }

    text.push_back(newTextCopy+'\n'+newText.substr(columnIdx));
}

void Meatball::TextBox::clearText() {
    text.clear();
}

const std::list<std::string>& Meatball::TextBox::getText() {
    return text;
}

void Meatball::TextBox::setFontSize(unsigned char newFontSize) {
    fontSize = newFontSize;
}

const unsigned char& Meatball::TextBox::getFontSize() {
    return fontSize;
}

void Meatball::TextBox::draw() {
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, color);

    size_t lineIdx = 0;
    for (auto& line : text) {
        if (lineIdx*fontSize >= rect.height) break;
        DrawText(line.c_str(), rect.x, rect.y+(lineIdx*fontSize), fontSize, textColor);
        lineIdx++;
    }
    
    scrollBar.draw();
}

void Meatball::TextBox::update() {
    scrollBar.update();
}