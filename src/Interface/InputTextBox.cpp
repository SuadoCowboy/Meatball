#include "InputTextBox.h"

Meatball::InputTextBox::InputTextBox() : color(BLACK), textColor(WHITE), cursorColor(WHITE), fontSize(16) {
    rect = (Rectangle){0,0,0,0};
}

Meatball::InputTextBox::InputTextBox(float x, float y, float width, float height, unsigned char fontSize)
 : color(BLACK), textColor(WHITE), cursorColor(WHITE), fontSize(fontSize) {
    rect = (Rectangle){x, y, width, height};
}

void Meatball::InputTextBox::draw() {
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, color);
    DrawText(text.c_str(), rect.x, rect.y+rect.height/2-fontSize/2, fontSize, textColor);
    
    // maybe cursor should be outside inputtextbox and be a function or class too like borders and stuff
    float x = rect.x+MeasureText(text.substr(0, cursorPos).c_str(), fontSize)+1;
    DrawLine(x, rect.y, x, rect.y+rect.height, cursorColor);
}

void Meatball::InputTextBox::update() {
    int codePoint = 0;
    while (codePoint = GetCharPressed(), codePoint)
        text.push_back((char)codePoint);
}