#include "ScrollTextBox.h"

#include "Utils/Utils.h"
#include "Utils/DrawFuncs.h"

Meatball::Config::ScrollTextBox Meatball::Defaults::scrollTextBoxConfig;

Meatball::Config::ScrollTextBox::ScrollTextBox() : font(nullptr), fontSize(0), color(BLACK) {}

Meatball::ScrollTextBox::ScrollTextBox() {
    setSize(0,0);
    setPosition(0,0);
}

Meatball::ScrollTextBox::ScrollTextBox(const Rectangle& rect) {
    setSize(rect.width, rect.height);
    setPosition(rect.x, rect.y);
}

static float _getContentHeight(const std::deque<Meatball::ColoredText> &text, unsigned short fontSize) {
    size_t lineIdx = 0;
    for (auto &line : text) {
        size_t newLineIdx = 0;
        while (newLineIdx != std::string::npos) {
            newLineIdx = line.text.find('\n', newLineIdx+1);
            lineIdx++;
        }
    }

    return lineIdx*fontSize; // contentHeight
}

static inline void handleTextWrapping(std::deque<Meatball::ColoredText> &textList, const std::string &text, const Color& color, Font* font, float maxWidth) {
    textList.emplace_back("", color);
    std::string newText = text;

    while (Meatball::measureTextWidth(*font, font->baseSize, newText.c_str()) >= maxWidth) {
        size_t columnIdx = 1; 
        
        while (Meatball::measureTextWidth(*font, font->baseSize, newText.substr(0, columnIdx).c_str()) < maxWidth)
            ++columnIdx;
        
        --columnIdx;

        textList.back().text += newText.substr(0, columnIdx)+"\n";
        newText = newText.substr(columnIdx);
    }

    textList.back().text += newText;
}

void Meatball::ScrollTextBox::updateTextWrap() {
    for (auto &currentText : text) {
        std::string newText = "";

        if (currentText.text.find('\n') != std::string::npos) {
            bool foundNonNewLine = false;
            
            for (auto &c : currentText.text) {
                if (c == '\n' && foundNonNewLine)
                    continue;

                newText += c;
                if (c != '\n') {
                    foundNonNewLine = true;
                }
            }

            currentText.text = newText;
        }

        newText.clear();
        while (Meatball::measureTextWidth(*config->font, config->fontSize, currentText.text.c_str()) >= rect.width-scrollBar.getRect().width) {
            size_t columnIdx = 1;

            while (Meatball::measureTextWidth(*config->font, config->fontSize, currentText.text.substr(0, columnIdx).c_str()) < rect.width-scrollBar.getRect().width)
                ++columnIdx;
            
            --columnIdx;
            if (columnIdx == 0) break;

            newText += currentText.text.substr(0, columnIdx)+"\n";
            currentText.text = currentText.text.substr(columnIdx);
        }

        newText += currentText.text;
        currentText.text = newText;
    }

    contentHeight = _getContentHeight(text, config->fontSize);
    scrollBar.updateThumbHeight(rect.height, contentHeight);
    scrollBar.update(rect);
    scrollBar.visible = contentHeight > rect.height;
}

void Meatball::ScrollTextBox::appendText(const std::string& newText, const Color& color) {
    if (newText.size() == 0) return;
    
    const Font& font = *config->font;
    float height = config->fontSize;
    const char* _text = newText.c_str();
    float width = Meatball::measureTextWidth(font, height, _text);
    if (width < rect.width-scrollBar.getRect().width) {
        text.emplace_back(newText, color);
    } else
        handleTextWrapping(text, newText, color, config->font, rect.width-scrollBar.getRect().width);


    contentHeight = _getContentHeight(text, config->fontSize);
    scrollBar.updateThumbHeight(rect.height, contentHeight);
    if (contentHeight > rect.height) scrollBar.visible = true;
}

void Meatball::ScrollTextBox::clearText() {
    text.clear();
    scrollBar.visible = false;
    
    // to fix view
    contentHeight = _getContentHeight(text, config->fontSize);
    scrollBar.updateThumbHeight(rect.height, contentHeight);
    scrollBar.update(rect);
}

void Meatball::ScrollTextBox::popFront() noexcept {
    text.pop_front();
    contentHeight = _getContentHeight(text, config->fontSize);
    scrollBar.updateThumbHeight(rect.height, contentHeight);
}

const std::deque<Meatball::ColoredText> &Meatball::ScrollTextBox::getText() const {
    return text;
}

unsigned int Meatball::ScrollTextBox::getContentHeight() const {
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
            newLineIdx = line.text.find('\n', newLineIdx+1);
            ++newLineAmount;
        }

        int lineY = lineIdx*config->fontSize-scrollBar.getScrollValue()*rect.height;

        if (lineY > rect.height) break;

        if (lineY+config->fontSize*newLineAmount > 0)
            drawText(*config->font, config->fontSize, line.text.c_str(), rect.x,
            rect.y+lineY+1/*+1 because letters get stuck 1 pixel in the top*/, line.color);
        
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