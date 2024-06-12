#include "ScrollTextBox.h"

#include "Utils/Utils.h"
#include "Utils/DrawFuncs.h"
#include "OutputColors.h"

Meatball::Config::ScrollTextBox Meatball::Defaults::scrollTextBoxConfig;

Meatball::Config::ScrollTextBox::ScrollTextBox() : font(nullptr), color(BLACK) {}

Meatball::ScrollTextBox::ScrollTextBox() {
    setSize(0,0);
    setPosition(0,0);

    scrollBar.config = std::make_shared<Config::ScrollBar>(Defaults::scrollBarConfig);
}

Meatball::ScrollTextBox::ScrollTextBox(const Rectangle& rect) {
    setSize(rect.width, rect.height);
    setPosition(rect.x, rect.y);

    scrollBar.config = std::make_shared<Config::ScrollBar>(Defaults::scrollBarConfig);
}

static float _getContentHeight(const std::list<std::pair<std::string, const std::shared_ptr<Color>>> &text, float fontSize) {
    size_t lineIdx = 0;
    for (auto &line : text) {
        size_t newLineIdx = 0;
        while (newLineIdx != std::string::npos) {
            newLineIdx = line.first.find('\n', newLineIdx+1);
            lineIdx++;
        }
    }

    return lineIdx*fontSize; // contentHeight
}

static inline void handleTextWrapping(std::list<std::pair<std::string, const std::shared_ptr<Color>>> &textList, const std::string &text, const std::shared_ptr<Color>& color, std::shared_ptr<Font> font, float maxWidth) {
    textList.push_back({"", color});
    std::string newText = text;

    while (Meatball::measureTextWidth(*font, font->baseSize, newText.c_str()) >= maxWidth) {
        size_t columnIdx = 1; 
        
        while (Meatball::measureTextWidth(*font, font->baseSize, newText.substr(0, columnIdx).c_str()) < maxWidth)
            ++columnIdx;
        
        --columnIdx;

        textList.back().first += newText.substr(0, columnIdx)+"\n";
        newText = newText.substr(columnIdx);
    }

    textList.back().first += newText;
}

void Meatball::ScrollTextBox::updateTextWrap() {
    for (auto &currentText : text) {
        std::string newText = "";

        if (currentText.first.find('\n') != std::string::npos) {
            bool foundNonNewLine = false;
            
            for (auto &c : currentText.first) {
                if (c == '\n' && foundNonNewLine)
                    continue;

                newText += c;
                if (c != '\n') {
                    foundNonNewLine = true;
                }
            }

            currentText.first = newText;
        }

        newText.clear();
        while (Meatball::measureTextWidth(*config->font, config->font->baseSize, currentText.first.c_str()) >= rect.width-scrollBar.getRect().width) {
            size_t columnIdx = 1;

            while (Meatball::measureTextWidth(*config->font, config->font->baseSize, currentText.first.substr(0, columnIdx).c_str()) < rect.width-scrollBar.getRect().width)
                ++columnIdx;
            
            --columnIdx;
            if (columnIdx == 0) break;

            newText += currentText.first.substr(0, columnIdx)+"\n";
            currentText.first = currentText.first.substr(columnIdx);
        }

        newText += currentText.first;
        currentText.first = newText;
    }

    contentHeight = _getContentHeight(text, config->font->baseSize);
    scrollBar.updateThumbHeight(rect.height, contentHeight);
    scrollBar.update(rect);
    scrollBar.visible = contentHeight > rect.height;
}

void Meatball::ScrollTextBox::appendText(std::string newText, const std::shared_ptr<Color>& color) {
    if (newText.size() == 0) return;
    
    if (Meatball::measureTextWidth(*config->font, config->font->baseSize, newText.c_str()) < rect.width-scrollBar.getRect().width) {
        text.push_back({newText, color});
    } else
        handleTextWrapping(text, newText, color, config->font, rect.width-scrollBar.getRect().width);


    contentHeight = _getContentHeight(text, config->font->baseSize);
    scrollBar.updateThumbHeight(rect.height, contentHeight);
    if (contentHeight > rect.height) scrollBar.visible = true;
}

void Meatball::ScrollTextBox::clearText() {
    text.clear();
    scrollBar.visible = false;
    
    // to fix view
    contentHeight = _getContentHeight(text, config->font->baseSize);
    scrollBar.updateThumbHeight(rect.height, contentHeight);
    scrollBar.update(rect);
}

void Meatball::ScrollTextBox::popFront() noexcept {
    text.pop_front();
    contentHeight = _getContentHeight(text, config->font->baseSize);
    scrollBar.updateThumbHeight(rect.height, contentHeight);
}

const std::list<std::pair<std::string, const std::shared_ptr<Color>>> &Meatball::ScrollTextBox::getText() const {
    return text;
}

const unsigned int &Meatball::ScrollTextBox::getContentHeight() const {
    return contentHeight;
}

const Rectangle &Meatball::ScrollTextBox::getRect() const {
    return rect;
}

Meatball::ScrollBar &Meatball::ScrollTextBox::getScrollBar() {
    return scrollBar;
}

void Meatball::ScrollTextBox::setPosition(float x, float y) {
    rect.x = x;
    rect.y = y;
    
    scrollBar.setPosition(rect.x+rect.width-scrollBar.getRect().width, y);
}

void Meatball::ScrollTextBox::setSize(float width, float height) {
    rect.width = width;
    rect.height = height;

    scrollBar.setSize(width*0.02, height);
}

void Meatball::ScrollTextBox::draw() {
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, config->color);

    BeginScissorMode(rect.x, rect.y, rect.width-scrollBar.getRect().width, rect.height);

    size_t lineIdx = 0;
    for (auto &line : text) {
        size_t newLineIdx = 0;
        size_t newLineAmount = 0;
        while (newLineIdx != std::string::npos) {
            newLineIdx = line.first.find('\n', newLineIdx+1);
            ++newLineAmount;
        }

        int lineY = lineIdx*config->font->baseSize-scrollBar.getScrollValue()*rect.height;

        if (lineY > rect.height) break;

        if (lineY+config->font->baseSize*newLineAmount > 0)
            drawText(*config->font, config->font->baseSize, line.first.c_str(), rect.x,
            rect.y+lineY+1/*+1 because letters get stuck 1 pixel in the top*/, *line.second);
        
        lineIdx += newLineAmount;
    }
    
    EndScissorMode();
}

void Meatball::ScrollTextBox::drawScrollbar() {
    scrollBar.draw();
}

void Meatball::ScrollTextBox::update() {
    if (scrollBar.visible) scrollBar.update(rect);
}