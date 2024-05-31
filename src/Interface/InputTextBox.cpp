#include "InputTextBox.h"

#include "FontsHandler.h"

#include "Console.h"

using fh = Meatball::FontsHandler;

static float getRealCursorPos(unsigned int cursorPos, Font* font, const std::string& text) {
    return fh::MeasureTextWidth(font, text.substr(0, cursorPos).c_str())+1;
}

Meatball::InputTextBox::InputTextBox()
    : color(BLACK), textColor(WHITE), cursorColor(WHITE), cursorPos(0),
    offsetX(0), focused(false), selectedTextStartIdx(std::string::npos), selectedTextFinalIdx(std::string::npos) {
    rect = {0,0,0,0};
    font = FontsHandler::get("default");
}

Meatball::InputTextBox::InputTextBox(float x, float y, float width, float height, Font* font)
 : color(BLACK), textColor(WHITE), cursorColor(WHITE), font(font), cursorPos(0),
 offsetX(0), focused(false), selectedTextStartIdx(std::string::npos), selectedTextFinalIdx(std::string::npos) {
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
        
        if (selectedTextStartIdx != std::string::npos && selectedTextStartIdx != selectedTextFinalIdx) {
            float selectedX, selectedWidth;
            
            if (selectedTextFinalIdx > selectedTextStartIdx) {
                selectedX = getRealCursorPos(selectedTextStartIdx, font, text);
                selectedWidth = getRealCursorPos(selectedTextFinalIdx, font, text)-selectedX;
            } else {
                selectedX = getRealCursorPos(selectedTextFinalIdx, font, text);
                selectedWidth = getRealCursorPos(selectedTextStartIdx, font, text)-selectedX;
            }
            
            DrawRectangle(rect.x-offsetX+selectedX, rect.y, selectedWidth, rect.height, {100,100,100,50});
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

    bool selectMode = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
    bool moveMode = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);

    if (moveMode) {
        if (IsKeyPressed(KEY_A) && text.size() != 0) {
            selectedTextStartIdx = 0;
            cursorPos = text.size();
            selectedTextFinalIdx = cursorPos;
        }

        if (IsKeyPressed(KEY_C) && selectedTextFinalIdx != std::string::npos)
            SetClipboardText(text.substr(selectedTextFinalIdx > selectedTextStartIdx? selectedTextStartIdx : selectedTextFinalIdx, selectedTextFinalIdx > selectedTextStartIdx? selectedTextFinalIdx-selectedTextStartIdx
             : selectedTextStartIdx-selectedTextFinalIdx).c_str());
        
        else if (IsKeyPressed(KEY_V)) {
            const char* clipboard = GetClipboardText();

            size_t i = 0;
            if (selectedTextFinalIdx != std::string::npos) {
                cursorPos = selectedTextFinalIdx > selectedTextStartIdx? selectedTextStartIdx : selectedTextFinalIdx;
                size_t delta = selectedTextFinalIdx > selectedTextStartIdx? selectedTextFinalIdx-selectedTextStartIdx
                : selectedTextStartIdx-selectedTextFinalIdx;
                
                for (size_t i = 0; i < delta; i++)
                    text.erase(text.begin()+cursorPos);
            
                selectedTextStartIdx = selectedTextFinalIdx = std::string::npos;

                while (clipboard[i] != '\0') {
                    text.insert(text.begin()+cursorPos, clipboard[i]);
                    ++i;
                    ++cursorPos;
                }

            } else {
                while (clipboard[i] != '\0') {
                    text.insert(text.begin()+cursorPos, clipboard[i]);
                    ++i;
                    ++cursorPos;
                }
            }
        }
    
    }
    
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT)) {
        if (selectMode && selectedTextStartIdx == std::string::npos)
            selectedTextStartIdx = cursorPos;

        if (moveMode) {
            while (cursorPos != 0 && text[cursorPos-1] == ' ') --cursorPos;
            while (cursorPos != 0 && text[cursorPos-1] != ' ') --cursorPos;
        } else if (!selectMode && selectedTextFinalIdx != std::string::npos) {
            cursorPos = selectedTextFinalIdx > selectedTextStartIdx? selectedTextStartIdx : selectedTextFinalIdx;

        } else if (cursorPos != 0) --cursorPos;
        
        if (selectMode)
            selectedTextFinalIdx = cursorPos;
        else
            selectedTextStartIdx = selectedTextFinalIdx = std::string::npos;
    
    } else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT)) {
        if (selectMode && selectedTextStartIdx == std::string::npos)
            selectedTextStartIdx = cursorPos;
        
        if (moveMode) {
            while (cursorPos != text.size() && text[cursorPos] == ' ') ++cursorPos;
            while (cursorPos != text.size() && text[cursorPos] != ' ') ++cursorPos;
        } else if (!selectMode && selectedTextFinalIdx != std::string::npos) {
            cursorPos = selectedTextFinalIdx > selectedTextStartIdx? selectedTextFinalIdx : selectedTextStartIdx;

        } else if (cursorPos != text.size()) ++cursorPos;
        
        if (selectMode)
            selectedTextFinalIdx = cursorPos;
        else {
            selectedTextStartIdx = std::string::npos;
            selectedTextFinalIdx = std::string::npos;
        }
    
    } else if ((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) && cursorPos != 0) {
        if (selectedTextFinalIdx != std::string::npos) {
            cursorPos = selectedTextFinalIdx > selectedTextStartIdx? selectedTextStartIdx : selectedTextFinalIdx;
            size_t delta = selectedTextFinalIdx > selectedTextStartIdx? selectedTextFinalIdx-selectedTextStartIdx
             : selectedTextStartIdx-selectedTextFinalIdx;
            
            for (size_t i = 0; i < delta; i++)
                text.erase(text.begin()+cursorPos);
            
            selectedTextStartIdx = selectedTextFinalIdx = std::string::npos;
        
        } else if (moveMode) {
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
        } else {
            text.erase(text.begin()+cursorPos-1);
            --cursorPos;
        }
        
        if (onTextChange) onTextChange(text);
    
    } else if ((IsKeyPressed(KEY_DELETE) || IsKeyPressedRepeat(KEY_DELETE)) && cursorPos != text.size()) {
        if (selectedTextFinalIdx != std::string::npos)
            selectedTextStartIdx = selectedTextFinalIdx = std::string::npos;
        
        if (moveMode) {
            // if already erased space, to not erase other chars
            bool usedSpaceVersion = false;
            
            while (cursorPos != text.size() && text[cursorPos] == ' ') {
                text.erase(text.begin()+cursorPos);
                usedSpaceVersion = true;
            }
            
            if (!usedSpaceVersion) while (cursorPos != text.size() && text[cursorPos] != ' ')
                text.erase(text.begin()+cursorPos);
        } else
            text.erase(text.begin()+cursorPos);
        
        if (onTextChange) onTextChange(text);
    }
    
    int codePoint = 0;
    if (codePoint = GetCharPressed(), codePoint) {
        if (selectedTextFinalIdx != std::string::npos) {
            cursorPos = selectedTextFinalIdx > selectedTextStartIdx? selectedTextStartIdx : selectedTextFinalIdx;
            size_t delta = selectedTextFinalIdx > selectedTextStartIdx? selectedTextFinalIdx-selectedTextStartIdx
             : selectedTextStartIdx-selectedTextFinalIdx;
            
            for (size_t i = 0; i < delta; i++)
                text.erase(text.begin()+cursorPos);
        }

        text.insert(text.begin()+cursorPos, (char)codePoint);
        if (onTextChange) onTextChange(text);
        
        ++cursorPos;

        selectedTextStartIdx = selectedTextFinalIdx = std::string::npos;
    }

    if (onSend && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) && text.size() != 0) {
        onSend(text);
        text.clear();
        cursorPos = text.size();
        offsetX = 0;
        selectedTextStartIdx = selectedTextFinalIdx = std::string::npos;
    
    } else {
        float x = getRealCursorPos(cursorPos, font, text);
        if (x-offsetX > rect.width)
            offsetX = x-rect.width+1;
        else if (x-offsetX < 0)
            offsetX = x-1;
    }
}