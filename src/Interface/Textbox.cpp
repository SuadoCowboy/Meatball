#include "TextBox.h"

#include "Utils/Utils.h"

Meatball::TextBox::TextBox()
    : rect((Rectangle){0,0,0,0}), fontSize(1), color(BLACK),
    textColor(WHITE), scrollBar((Rectangle){0,0,0,0}) {}

Meatball::TextBox::TextBox(Rectangle rect, unsigned char fontSize)
    : rect(rect), fontSize(fontSize), color(BLACK), textColor(WHITE),
    scrollBar((Rectangle){rect.x+rect.width-20,rect.y, 20, rect.height}) {}

static inline void handleTextWrapping(std::list<std::string>& textList, const std::string& text, const float& fontSize, float maxWidth) {
    textList.push_back("");
    
    std::string newText = text;

    while (MeasureText(newText.c_str(), fontSize) >= maxWidth) {
        std::string newTextCopy = newText;
        size_t columnIdx = newTextCopy.size()-1;
        
        while (MeasureText(newTextCopy.c_str(), fontSize) >= maxWidth) {
            newTextCopy = newText.substr(0, columnIdx);
            columnIdx--;
        }

        textList.back() += newTextCopy+"\n";
        newText = newText.substr(columnIdx);
    }

    textList.back() += newText;
}

void Meatball::TextBox::appendText(std::string newText) {
    if (newText.size() == 0) return;
    
    if (MeasureText(newText.c_str(), fontSize) < rect.width-scrollBar.barRect.width) {
        text.push_back(newText);
    } else
        handleTextWrapping(text, newText, fontSize, rect.width-scrollBar.barRect.width);
    
    scrollBar.updateThumbHeight(rect.height, getContentHeight(rect.height, fontSize, text));
}

void Meatball::TextBox::clearText() {
    text.clear();
    scrollBar.visible = false;
    
    // to fix view
    scrollBar.updateThumbHeight(rect.height, getContentHeight(rect.height, fontSize, text));
    scrollBar.update(rect);
}

void Meatball::TextBox::popFront() noexcept {
    text.pop_front();
    scrollBar.updateThumbHeight(rect.height, getContentHeight(rect.height, fontSize, text));
}

const std::list<std::string>& Meatball::TextBox::getText() const {
    return text;
}

void Meatball::TextBox::setFontSize(unsigned char newFontSize) {
    fontSize = newFontSize;
}

const unsigned char& Meatball::TextBox::getFontSize() const {
    return fontSize;
}

const Rectangle& Meatball::TextBox::getRect() const {
    return rect;
}

Meatball::ScrollBar& Meatball::TextBox::getScrollBar() {
    return scrollBar;
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
        int lineY = lineIdx*fontSize-scrollBar.getScrollValue()*rect.height;

        if (lineY > rect.height) break;
        
        if (lineY+fontSize > 0)
            DrawText(line.c_str(), rect.x, rect.y+(float)lineY, fontSize, textColor);
        
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
    scrollBar.update(rect);
}