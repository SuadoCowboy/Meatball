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
        rect.y+rect.height/2-font->baseSize/2+1/*+1 because it sticks 1 pixel on top*/,
        textColor);
    
    // maybe cursor should be outside inputtextbox and be a function or class too like borders and stuff
    if (focused) {
        float x = rect.x-offsetX+getRealCursorPos(cursorPos, font, text);
        DrawLine(x, rect.y, x, rect.y+rect.height, cursorColor);
    }

    EndScissorMode();
}

void Meatball::InputTextBox::update() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        bool wasFocusedBefore = focused;
        focused = CheckCollisionPointRec(GetMousePosition(), rect);
        
        if (wasFocusedBefore && focused && text.size() != 0) {
            unsigned int newCursorPos = 0;
            float textWidth = 0;
            
            while (newCursorPos < text.size() && textWidth < GetMouseX()-rect.x+offsetX) {
                textWidth += fh::MeasureTextWidth(font, text.substr(newCursorPos, 1).c_str())+1;
                ++newCursorPos;
            }
            
            cursorPos = newCursorPos;
        }
    }

    if (!focused) return;

    if (IsKeyDown(KEY_LEFT_CONTROL)) { // Move
        if (cursorPos != 0) { // left logic
            if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT)) {
                while (cursorPos != 0 && text[cursorPos-1] == ' ') --cursorPos; // skip whitespaces to go to word if there's any
                while (cursorPos != 0 && text[cursorPos-1] != ' ') --cursorPos; // skip the word until end of the word
            
            } else if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) {
                // if already erased space, to not erase other chars
                bool usedSpaceVersion = false;
                
                while (cursorPos != 0 && text[cursorPos-1] == ' ') {
                    text.erase(text.begin()+cursorPos-1);
                    --cursorPos;
                    usedSpaceVersion = true;
                }
                
                if (!usedSpaceVersion) while (cursorPos != 0 && text[cursorPos-1] != ' ') {
                    text.erase(text.begin()+cursorPos-1);
                    --cursorPos;
                }
                
                if (onTextChange) onTextChange(text);
            }
        }
        if (cursorPos != text.size()) { // right logic
            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT)) {
                while (cursorPos != text.size() && text[cursorPos] == ' ') ++cursorPos;
                while (cursorPos != text.size() && text[cursorPos] != ' ') ++cursorPos;
            
            } else if (IsKeyPressed(KEY_DELETE) || IsKeyPressedRepeat(KEY_DELETE)) {
                // if already erased space, to not erase other chars
                bool usedSpaceVersion = false;
                
                while (cursorPos != text.size() && text[cursorPos] == ' ') {
                    text.erase(text.begin()+cursorPos);
                    usedSpaceVersion = true;
                }
                
                if (!usedSpaceVersion) while (cursorPos != text.size() && text[cursorPos] != ' ')
                    text.erase(text.begin()+cursorPos);
                
                if (onTextChange) onTextChange(text);
            }
        }
        
        // CTRL+A/LEFT/RIGHT/ (MAYBE)+SHIFT+LEFT/RIGHT
        /*
        Possibilities:
        A = SELECT ALL(todo select logic)
        */
    } else if (IsKeyDown(KEY_LEFT_SHIFT)) { // Select

    }

    else if ((IsKeyPressedRepeat(KEY_LEFT) || IsKeyPressed(KEY_LEFT)) && cursorPos != 0)
        --cursorPos;
    else if ((IsKeyPressedRepeat(KEY_RIGHT) || IsKeyPressed(KEY_RIGHT)) && cursorPos != text.size())
        ++cursorPos;

    else if ((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) && cursorPos != 0) {
        text.erase(text.begin()+cursorPos-1);
        --cursorPos;
        
        if (onTextChange) onTextChange(text);
    }

    else if ((IsKeyPressed(KEY_DELETE) || IsKeyPressedRepeat(KEY_DELETE)) && cursorPos != text.size()) {
        text.erase(text.begin()+cursorPos);
        if (onTextChange) onTextChange(text);
    }
    
    int codePoint = 0;
    if (codePoint = GetCharPressed(), codePoint) {
        text.insert(text.begin()+cursorPos, (char)codePoint);
        if (onTextChange) onTextChange(text);
        
        ++cursorPos;
    }

    if (onSend && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) && text.size() != 0) {
        onSend(text);
        text.clear();
        cursorPos = text.size();
        offsetX = 0;
    } else {
        float x = getRealCursorPos(cursorPos, font, text);
        if (x-offsetX > rect.width)
            offsetX = x-rect.width+1;
        else if (x-offsetX < 0)
            offsetX = x-1;
    }
}