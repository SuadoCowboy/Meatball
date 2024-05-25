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
        size_t columnIdx = 1; 
        
        while (MeasureText(newText.substr(0, columnIdx).c_str(), fontSize) < maxWidth)
            ++columnIdx;
        
        --columnIdx;

        textList.back() += newText.substr(0, columnIdx)+"\n";
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
    
    contentHeight = Meatball::getContentHeight(rect.height, fontSize, text);
    scrollBar.updateThumbHeight(rect.height, contentHeight);
}

void Meatball::TextBox::clearText() {
    text.clear();
    scrollBar.visible = false;
    
    // to fix view
    contentHeight = Meatball::getContentHeight(rect.height, fontSize, text);
    scrollBar.updateThumbHeight(rect.height, contentHeight);
    scrollBar.update(rect);
}

void Meatball::TextBox::popFront() noexcept {
    text.pop_front();
    contentHeight = Meatball::getContentHeight(rect.height, fontSize, text);
    scrollBar.updateThumbHeight(rect.height, contentHeight);
}

const std::list<std::string>& Meatball::TextBox::getText() const {
    return text;
}

const unsigned int Meatball::TextBox::getContentHeight() const {
    return contentHeight;
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
        size_t newLineIdx = 0;
        size_t newLineAmount = 0;
        while (newLineIdx != std::string::npos) {
            newLineIdx = line.find('\n', newLineIdx+1);
            ++newLineAmount;
        }

        int lineY = lineIdx*fontSize-scrollBar.getScrollValue()*rect.height;

        if (lineY > rect.height) break;

        if (lineY+fontSize*newLineAmount > 0)
            DrawText(line.c_str(), rect.x, rect.y+(float)lineY, fontSize, textColor);
        
        lineIdx += newLineAmount;
    }
    
    EndScissorMode();
    
    scrollBar.draw();
}

void Meatball::TextBox::update() {
    scrollBar.update(rect);
}