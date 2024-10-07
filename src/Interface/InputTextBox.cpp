#include "InputTextBox.h"

#include "Utils/DrawFuncs.h"
#include "Utils/Utils.h"
#include "Console.h"
#include "MouseCursor.h"

static float getRealCursorPos(unsigned int cursorPos, const Font& font, float fontHeight, const std::string& text) {
    return Meatball::measureTextWidth(font, fontHeight, text.substr(0, cursorPos).c_str())+1;
}

Meatball::InputTextBox::InputTextBox() {}

Meatball::InputTextBox::InputTextBox(const Rectangle& rect, unsigned short fontSize)
 : rect(rect), fontSize(fontSize) {}

void Meatball::InputTextBox::draw(const Font& font, const Color& textColor, const Color& cursorColor, const Color& selectionColor) const {
    BeginScissorMode(rect.x-1, rect.y, rect.width, rect.height);

    float textY = rect.y+(rect.height-fontSize)*0.5;
    drawText(font, fontSize, text.c_str(),
        rect.x-offsetX,
        textY,
        textColor);
    
    // maybe cursor should be outside inputtextbox and be a function too in drawFuncs.h
    if (focused) {
        float x = rect.x-offsetX+getRealCursorPos(cursorPos, font, fontSize, text);
        DrawLine(x, textY, x, textY+fontSize, cursorColor);
        
        if (selectedTextStartIdx != textMaxSize+1 && selectedTextFinalIdx != textMaxSize+1) {
            float selectedX, selectedWidth;
            
            if (selectedTextFinalIdx > selectedTextStartIdx) {
                selectedX = getRealCursorPos(selectedTextStartIdx, font, fontSize, text);
                selectedWidth = getRealCursorPos(selectedTextFinalIdx, font, fontSize, text)-selectedX;
            } else {
                selectedX = getRealCursorPos(selectedTextFinalIdx, font, fontSize, text);
                selectedWidth = getRealCursorPos(selectedTextStartIdx, font, fontSize, text)-selectedX;
            }
            
            DrawRectangle(rect.x-offsetX+selectedX, textY, selectedWidth, fontSize, selectionColor);
        }
    }

    EndScissorMode();
}

bool Meatball::InputTextBox::setText(const std::string& newText) {
    if (newText.size() > textMaxSize) return false;

    text = newText;
    return true;
}

const std::string& Meatball::InputTextBox::getText() const {
    return text;
}

void Meatball::InputTextBox::onKeyboardPress(int key, const Font& font) {
    if (!focused) return;

    size_t textSize = text.size();

    bool selectMode = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
    bool moveMode = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);

    if (moveMode) {
        if (IsKeyPressed(KEY_A) && textSize != 0) {
            selectedTextStartIdx = 0;
            cursorPos = textSize;
            selectedTextFinalIdx = cursorPos;
        }

        if (IsKeyPressed(KEY_C) && selectedTextFinalIdx != textMaxSize+1)
            SetClipboardText(text.substr(selectedTextFinalIdx > selectedTextStartIdx? selectedTextStartIdx : selectedTextFinalIdx, selectedTextFinalIdx > selectedTextStartIdx? selectedTextFinalIdx-selectedTextStartIdx
             : selectedTextStartIdx-selectedTextFinalIdx).c_str());
        
        else if ((IsKeyPressed(KEY_V) || IsKeyPressedRepeat(KEY_V))) {
            if (selectedTextFinalIdx <= textSize) {
                // cursorPos = left idx
                cursorPos = selectedTextFinalIdx > selectedTextStartIdx? selectedTextStartIdx : selectedTextFinalIdx;
                // cursorPos+delta = right idx
                unsigned short delta = selectedTextFinalIdx > selectedTextStartIdx? selectedTextFinalIdx-selectedTextStartIdx
                : selectedTextStartIdx-selectedTextFinalIdx;
                
                // erase selected text
                for (unsigned short i = 0; i < delta; ++i)
                    text.erase(text.begin()+cursorPos);
                
                textSize = text.size();

                selectedTextStartIdx = selectedTextFinalIdx = textMaxSize+1;
            }

            const char* clipboard = GetClipboardText();
            for (size_t i = 0; clipboard[i] != '\0'; ++i) {
                if (textSize >= textMaxSize) break;
                
                text.insert(text.begin()+cursorPos, clipboard[i]);
                ++cursorPos;

                textSize = text.size();
            }

        }
    }
    
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT)) {
        if (selectMode && selectedTextStartIdx == textMaxSize+1)
            selectedTextStartIdx = cursorPos;

        if (moveMode) {
            while (cursorPos != 0 && text[cursorPos-1] == ' ') --cursorPos;
            while (cursorPos != 0 && text[cursorPos-1] != ' ') --cursorPos;
        } else if (!selectMode && selectedTextFinalIdx != textMaxSize+1 && selectedTextStartIdx != selectedTextFinalIdx) {
            cursorPos = selectedTextFinalIdx >= selectedTextStartIdx? selectedTextStartIdx : selectedTextFinalIdx;

        } else if (cursorPos != 0) --cursorPos;
        
        if (selectMode)
            selectedTextFinalIdx = cursorPos;
        else
            selectedTextStartIdx = selectedTextFinalIdx = textMaxSize+1;
    
    } else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT)) {
        if (selectMode && selectedTextStartIdx == textMaxSize+1)
            selectedTextStartIdx = cursorPos;
        
        if (moveMode) {
            while (cursorPos != textSize && text[cursorPos] == ' ') ++cursorPos;
            while (cursorPos != textSize && text[cursorPos] != ' ') ++cursorPos;
        } else if (!selectMode && selectedTextFinalIdx != textMaxSize+1 && selectedTextStartIdx != selectedTextFinalIdx) {
            cursorPos = selectedTextFinalIdx >= selectedTextStartIdx? selectedTextFinalIdx : selectedTextStartIdx;

        } else if (cursorPos != textSize) ++cursorPos;
        
        if (selectMode)
            selectedTextFinalIdx = cursorPos;
        else {
            selectedTextStartIdx = selectedTextFinalIdx = textMaxSize+1;
        }
    
    } else if ((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) && cursorPos != 0) {
        if (selectedTextFinalIdx != textMaxSize+1) {
            cursorPos = selectedTextFinalIdx > selectedTextStartIdx? selectedTextStartIdx : selectedTextFinalIdx;
            unsigned short delta = selectedTextFinalIdx > selectedTextStartIdx? selectedTextFinalIdx-selectedTextStartIdx
             : selectedTextStartIdx-selectedTextFinalIdx;
            
            for (unsigned short i = 0; i < delta; ++i)
                text.erase(text.begin()+cursorPos);
            
            textSize = text.size();
            
            selectedTextStartIdx = selectedTextFinalIdx = textMaxSize+1;
        
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

            textSize = text.size();
        
        } else {
            text.erase(text.begin()+cursorPos-1);
            --cursorPos;
            textSize = text.size();
        }
        
        if (onTextChange) onTextChange(text);
    
    } else if ((IsKeyPressed(KEY_DELETE) || IsKeyPressedRepeat(KEY_DELETE)) && cursorPos != textSize) {
        if (selectedTextFinalIdx != textMaxSize+1)
            selectedTextStartIdx = selectedTextFinalIdx = textMaxSize+1;
        
        if (moveMode) {
            // if already erased space, to not erase other chars
            bool usedSpaceVersion = false;
            
            while (cursorPos != textSize && text[cursorPos] == ' ') {
                text.erase(text.begin()+cursorPos);
                usedSpaceVersion = true;
            }
            
            if (!usedSpaceVersion) while (cursorPos != textSize && text[cursorPos] != ' ')
                text.erase(text.begin()+cursorPos);
            
            textSize = text.size();
        } else {
           text.erase(text.begin()+cursorPos);
           textSize = text.size();
        }
        
        if (onTextChange) onTextChange(text);
    }

    if (onSend && (key == KEY_ENTER || key == KEY_KP_ENTER) && textSize != 0) {
        onSend(text);
        text.clear();
        cursorPos = textSize = offsetX = 0;
        selectedTextStartIdx = selectedTextFinalIdx = textMaxSize+1;

    } else {
        float x = getRealCursorPos(cursorPos, font, fontSize, text);
        if (x-offsetX > rect.width || x-offsetX < 0) {
            offsetX = x-rect.width+1;
            if (offsetX < 0) offsetX = 0;
        }
    }
}

void Meatball::InputTextBox::onCharPress(int codePoint) {
    if (!focused) return;
    
    if (selectedTextFinalIdx != textMaxSize+1) {
        cursorPos = selectedTextFinalIdx > selectedTextStartIdx? selectedTextStartIdx : selectedTextFinalIdx;
        unsigned short delta = selectedTextFinalIdx > selectedTextStartIdx? selectedTextFinalIdx-selectedTextStartIdx
            : selectedTextStartIdx-selectedTextFinalIdx;
        
        for (unsigned short i = 0; i < delta; ++i)
            text.erase(text.begin()+cursorPos);
    }

    if (text.size() < textMaxSize) {
        text.insert(text.begin()+cursorPos, (char)codePoint);
        if (onTextChange) onTextChange(text);

        ++cursorPos;
        
        selectedTextStartIdx = selectedTextFinalIdx = textMaxSize+1;
    }
}

void Meatball::InputTextBox::onMousePress(int button, const Font& font) {
    size_t textSize = text.size();
    
    if (button == MOUSE_BUTTON_LEFT) {
        bool wasFocusedBefore = focused;
        focused = hovered;
        
        if (wasFocusedBefore && focused && textSize != 0) {
            unsigned int newCursorPos = 0;
            float textWidth = 0;
            
            while (newCursorPos < textSize && textWidth < GetMouseX()-rect.x+offsetX) {
                textWidth += Meatball::measureTextWidth(font, fontSize, text.substr(newCursorPos, 1).c_str())+1;
                ++newCursorPos;
            }
            
            cursorPos = newCursorPos;
            selectedTextStartIdx = selectedTextFinalIdx = textMaxSize+1;
            mousePressed = true;
        }
    }

    if (mousePressed && textSize != 0) {
        if (selectedTextStartIdx == textMaxSize+1)
            selectedTextStartIdx = cursorPos;
        
        float newCursorPos = 0;
        float textWidth = 0;
        
        while (newCursorPos < textSize && textWidth < GetMouseX()-rect.x+offsetX) {
            textWidth += Meatball::measureTextWidth(font, fontSize, text.substr(newCursorPos, 1).c_str())+1;
            ++newCursorPos;
        }
        
        cursorPos = newCursorPos;
        selectedTextFinalIdx = cursorPos;
    }
}

void Meatball::InputTextBox::onMouseRelease(int button) {
    if (button == MOUSE_BUTTON_LEFT)
        mousePressed = false;
}

void Meatball::InputTextBox::onMouseMove(const Vector2& mousePosition) {
    hovered = CheckCollisionPointRec(mousePosition, rect);
    
    if (hovered)
        setCursor(MOUSE_CURSOR_IBEAM, MouseCursorPriorityLevel::INPUT_TEXT_BOX);
    else
        resetCursor(MouseCursorPriorityLevel::INPUT_TEXT_BOX);
}