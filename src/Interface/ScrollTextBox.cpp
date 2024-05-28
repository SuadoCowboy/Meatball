#include "ScrollTextBox.h"

#include "Utils/Utils.h"
#include "FontsHandler.h"

using fh = Meatball::FontsHandler;

Meatball::ScrollTextBox::ScrollTextBox()
    : rect((Rectangle){0,0,0,0}), font(nullptr), color(BLACK),
    textColor(WHITE), scrollBar((Rectangle){0,0,0,0}) {
        font = FontsHandler::get("default");
    }

Meatball::ScrollTextBox::ScrollTextBox(float x, float y, float width, float height, Font* font)
    : font(font), color(BLACK), textColor(WHITE) {
        rect = (Rectangle{x,y,width,height});
        scrollBar = {(Rectangle){rect.x+rect.width-20,rect.y, 20, rect.height}, false};
}

static inline void handleTextWrapping(std::list<std::string>& textList, const std::string& text, Font* font, float maxWidth) {
    textList.push_back("");
    std::string newText = text;

    while (fh::MeasureTextWidth(font, newText.c_str()) >= maxWidth) {
        size_t columnIdx = 1; 
        
        while (fh::MeasureTextWidth(font, newText.substr(0, columnIdx).c_str()) < maxWidth)
            ++columnIdx;
        
        --columnIdx;

        textList.back() += newText.substr(0, columnIdx)+"\n";
        newText = newText.substr(columnIdx);
    }

    textList.back() += newText;
}

void Meatball::ScrollTextBox::appendText(std::string newText) {
    if (newText.size() == 0) return;
    
    if (fh::MeasureTextWidth(font, newText.c_str()) < rect.width-scrollBar.barRect.width) {
        text.push_back(newText);
    } else
        handleTextWrapping(text, newText, font, rect.width-scrollBar.barRect.width);
    
    contentHeight = Meatball::getContentHeight(rect.height, (float)font->baseSize, text);
    scrollBar.updateThumbHeight(rect.height, contentHeight);
    if (contentHeight > rect.height) scrollBar.visible = true;
}

void Meatball::ScrollTextBox::clearText() {
    text.clear();
    scrollBar.visible = false;
    
    // to fix view
    contentHeight = Meatball::getContentHeight(rect.height, (float)font->baseSize, text);
    scrollBar.updateThumbHeight(rect.height, contentHeight);
    scrollBar.update(rect);
}

void Meatball::ScrollTextBox::popFront() noexcept {
    text.pop_front();
    contentHeight = Meatball::getContentHeight(rect.height, (float)font->baseSize, text);
    scrollBar.updateThumbHeight(rect.height, contentHeight);
}

const std::list<std::string>& Meatball::ScrollTextBox::getText() const {
    return text;
}

const unsigned int Meatball::ScrollTextBox::getContentHeight() const {
    return contentHeight;
}

const Rectangle& Meatball::ScrollTextBox::getRect() const {
    return rect;
}

Meatball::ScrollBar& Meatball::ScrollTextBox::getScrollBar() {
    return scrollBar;
}

void Meatball::ScrollTextBox::setPosition(float x, float y) {
    rect.x = x;
    rect.y = y;
}

void Meatball::ScrollTextBox::setSize(float width, float height) {
    rect.width = width;
    rect.height = height;
}

void Meatball::ScrollTextBox::draw() {
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

        int lineY = lineIdx*(float)font->baseSize-scrollBar.getScrollValue()*rect.height;

        if (lineY > rect.height) break;

        if (lineY+(float)font->baseSize*newLineAmount > 0)
            fh::DrawText(font, line.c_str(), rect.x,
            rect.y+(float)lineY+1/*+1 because letters get stuck 1 pixel in the top*/, textColor);
        
        lineIdx += newLineAmount;
    }
    
    EndScissorMode();
    
    scrollBar.draw();
}

void Meatball::ScrollTextBox::update() {
    if (scrollBar.visible) scrollBar.update(rect);
}