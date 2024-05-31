#include "InputTextBox.h"

#include "FontsHandler.h"

#include "Console.h"

using fh = Meatball::FontsHandler;

static float getRealCursorPos(unsigned int cursorPos, Font* font, const std::string& text) {
    return fh::MeasureTextWidth(font, text.substr(0, cursorPos).c_str())+1;
}

Meatball::InputTextBox::InputTextBox()
    : color(BLACK), textColor(WHITE), cursorColor(WHITE), cursorPos(0),
    offsetX(0), focused(false), selectedTextBeginIdx(std::string::npos), selectedTextEndIdx(std::string::npos) {
    rect = {0,0,0,0};
    font = FontsHandler::get("default");
}

Meatball::InputTextBox::InputTextBox(float x, float y, float width, float height, Font* font)
 : color(BLACK), textColor(WHITE), cursorColor(WHITE), font(font), cursorPos(0),
 offsetX(0), focused(false), selectedTextBeginIdx(std::string::npos), selectedTextEndIdx(std::string::npos) {
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
        
        if (selectedTextBeginIdx != std::string::npos && selectedTextBeginIdx != selectedTextEndIdx) {
            float selectedX = getRealCursorPos(selectedTextBeginIdx, font, text);
            DrawRectangle(rect.x+selectedX, rect.y, getRealCursorPos(selectedTextEndIdx, font, text)-selectedX, rect.height, {100,100,100,50});
        }
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

    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) { // Move
        bool selectMode = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);

        if (cursorPos != 0) { // left logic
            if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT)) {
                // conditions are: shift key down;
                // endIdx/beginIdx not set yet OR they are equal(in same position)
                if (selectMode && selectedTextEndIdx == selectedTextBeginIdx)
                    selectedTextEndIdx = cursorPos;

                while (cursorPos != 0 && text[cursorPos-1] == ' ') --cursorPos; // skip whitespaces to go to word if there's any
                while (cursorPos != 0 && text[cursorPos-1] != ' ') --cursorPos; // skip the word until end of the word
                
                if (selectMode) { // aaa bbb
                    if (selectedTextBeginIdx > cursorPos)
                        selectedTextBeginIdx = cursorPos;
                    else selectedTextEndIdx = cursorPos;
                } else {
                    selectedTextBeginIdx = std::string::npos;
                    selectedTextEndIdx = std::string::npos;
                }

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
                // conditions are: shift key down;
                // endIdx/beginIdx not set yet OR they are equal(in same position)
                if (selectMode && selectedTextBeginIdx == selectedTextEndIdx)
                    selectedTextBeginIdx = cursorPos;

                while (cursorPos != text.size() && text[cursorPos] == ' ') ++cursorPos;
                while (cursorPos != text.size() && text[cursorPos] != ' ') ++cursorPos;
                
                if (selectMode) {
                    if (selectedTextEndIdx < cursorPos)
                        selectedTextEndIdx = cursorPos;
                    else selectedTextBeginIdx = cursorPos;
                } else {
                    selectedTextBeginIdx = std::string::npos;
                    selectedTextEndIdx = std::string::npos;
                }
            
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
        
        // CTRL+A/SHIFT+LEFT/RIGHT
        /*
        Possibilities:
        A = SELECT ALL(todo select logic)
        */
    
    } else if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) { // Select

    }

    else if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT)) {
        if (cursorPos != 0) --cursorPos;
        selectedTextBeginIdx = std::string::npos;
        selectedTextEndIdx = std::string::npos;
    }
    else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT)) {
        if (cursorPos != text.size()) ++cursorPos;
        selectedTextBeginIdx = std::string::npos;
        selectedTextEndIdx = std::string::npos;
    }

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