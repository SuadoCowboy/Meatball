#include "InputTextBox.h"

#include "FontsHandler.h"

#include "Console.h"

using fh = Meatball::FontsHandler;

static float getRealCursorPos(unsigned int cursorPos, Font* font, const std::string& text) {
    return fh::MeasureTextWidth(font, text.substr(0, cursorPos).c_str())+1;
}

Meatball::InputTextBox::InputTextBox()
    : color(BLACK), textColor(WHITE), cursorColor(WHITE), cursorPos(0),
    offsetX(0), focused(false) {
    rect = {0,0,0,0};
    font = FontsHandler::get("default");
}

Meatball::InputTextBox::InputTextBox(float x, float y, float width, float height, Font* font)
 : color(BLACK), textColor(WHITE), cursorColor(WHITE), font(font), cursorPos(0),
 offsetX(0), focused(false) {
    rect = {x, y, width, height};
}

void Meatball::InputTextBox::draw() {
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, color);
    BeginScissorMode(rect.x-1, rect.y, rect.width, rect.height);

    fh::DrawText(font, text.c_str(),
        rect.x-offsetX,
        rect.y+rect.height/2-(float)font->baseSize/2+1/*+1 because it sticks 1 pixel on top*/,
        textColor);
    
    // maybe cursor should be outside inputtextbox and be a function or class too like borders and stuff
    if (focused) {
        float x = rect.x-offsetX+getRealCursorPos(cursorPos, font, text);
        DrawLine(x, rect.y, x, rect.y+rect.height, cursorColor);
    }

    EndScissorMode();
}

void Meatball::InputTextBox::update() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        focused = CheckCollisionPointRec(GetMousePosition(), rect);

    if (!focused) return;
    
    int codePoint = 0;
    if (codePoint = GetCharPressed(), codePoint) {
        text = text.substr(0, cursorPos)+(char)codePoint+text.substr(cursorPos);
        if (onTextChange) onTextChange(text);
        
        if (cursorPos != text.size())
            ++cursorPos;
        
        float x = getRealCursorPos(cursorPos, font, text);
        if (x-offsetX > rect.width)
            offsetX = x-rect.width+1;
    }
    
    if ((IsKeyPressedRepeat(KEY_BACKSPACE) || IsKeyPressed(KEY_BACKSPACE)) && text.size() != 0) {
        text = text.substr(0, cursorPos-1)+text.substr(cursorPos);
        if (onTextChange) onTextChange(text);

        if (cursorPos != 0)
            --cursorPos;
        
        float x = getRealCursorPos(cursorPos, font, text);
        if (x-offsetX < 0) {
            offsetX = x-rect.width;
            if (offsetX < 0) offsetX = 0;
        }
    }

    if (onSend && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) && text.size() != 0) {
        onSend(text);
        text.clear();
        cursorPos = text.size();
        offsetX = 0;
    }

    // TODO: mouse click detection

    if ((IsKeyPressedRepeat(KEY_LEFT) || IsKeyPressed(KEY_LEFT)) && cursorPos != 0)
        --cursorPos;
    if ((IsKeyPressedRepeat(KEY_RIGHT) || IsKeyPressed(KEY_RIGHT)) && cursorPos != text.size())
        ++cursorPos;
}