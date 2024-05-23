#include "TextBox.h"

#include "Utils/Utils.h"

Meatball::TextBox::TextBox()
    : rect((Rectangle){0,0,0,0}), fontSize(1), color(BLACK),
    textColor(WHITE), scrollBar((Rectangle){0,0,0,0}) {}

Meatball::TextBox::TextBox(Rectangle rect, unsigned char fontSize)
    : rect(rect), fontSize(fontSize), color(BLACK), textColor(WHITE),
    scrollBar((Rectangle){rect.x+rect.width-20,rect.y, 20, rect.height}) {}

void Meatball::TextBox::appendText(std::string newText) {
    if (newText.size() == 0) return;
    while (std::isspace(newText[newText.size()-1])) newText = newText.substr(0, newText.size()-1);

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

const Rectangle& Meatball::TextBox::getRect() {
    return rect;
}

void Meatball::TextBox::setPosition(float x, float y) {
    rect.x = x;
    rect.y = y;
}

void Meatball::TextBox::setSize(float width, float height) {
    rect.width = width;
    rect.height = height;
}

void Meatball::TextBox::draw() {
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, color);

    BeginScissorMode(rect.x, rect.y, rect.width, rect.height);
    
    size_t lineIdx = 0;
    for (auto& line : text) {
        float lineY = lineIdx*fontSize-scrollBar.getScrollY()*scrollBar.getScrollHeight()/2;

        if (lineY > rect.height) break;
        
        if (lineY+fontSize > 0)
            DrawText(line.c_str(), rect.x, rect.y+lineY, fontSize, textColor);
        
        size_t newLineIdx = 0;
        while (newLineIdx != std::string::npos) {
            newLineIdx = line.find('\n', newLineIdx+1);
            lineIdx++;
        }
    }
    
    EndScissorMode();
    
    scrollBar.draw();
}

void Meatball::TextBox::update() {
    scrollBar.update();

    if (CheckCollisionPointRec(GetMousePosition(), rect))
        scrollBar.updateWheelScroll();
}