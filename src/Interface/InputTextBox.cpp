#include "InputTextBox.h"

#include "FontsHandler.h"

using fh = Meatball::FontsHandler;

Meatball::InputTextBox::InputTextBox() : color(BLACK), textColor(WHITE), cursorColor(WHITE), font(nullptr) {
    rect = (Rectangle){0,0,0,0};
    font = FontsHandler::get("default");
}

Meatball::InputTextBox::InputTextBox(float x, float y, float width, float height, Font* font)
 : color(BLACK), textColor(WHITE), cursorColor(WHITE), font(font) {
    rect = (Rectangle){x, y, width, height};
}

void Meatball::InputTextBox::draw() {
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, color);
    fh::DrawText(font, text.c_str(), rect.x, rect.y+rect.height/2-(float)font->baseSize/2, textColor);
    
    // maybe cursor should be outside inputtextbox and be a function or class too like borders and stuff
    float x = rect.x+fh::MeasureTextWidth(font, text.substr(0, cursorPos).c_str())+1;
    DrawLine(x, rect.y, x, rect.y+rect.height, cursorColor);
}

void Meatball::InputTextBox::update() {
    int codePoint = 0;
    while (codePoint = GetCharPressed(), codePoint) {
        text.push_back((char)codePoint);
        onTextChange(text);
    }
    
    if ((IsKeyPressedRepeat(KEY_BACKSPACE) || IsKeyPressed(KEY_BACKSPACE)) && text.size() != 0) {
        text.pop_back();
        onTextChange(text);
    }

    if (onSend && IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
        onSend(text);
        text.clear();
    }
}