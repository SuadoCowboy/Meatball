#include "InputTextBox.h"

#include "FontsHandler.h"
#include "Utils/DrawFuncs.h"
#include "Utils/Utils.h"
#include "Console.h"

using fh = Meatball::FontsHandler;

std::shared_ptr<Meatball::Config::InputTextBox> Meatball::Defaults::inputTextBoxConfig;

Meatball::Config::InputTextBox::InputTextBox()
  : color({40,40,40,255}), textColor(WHITE), cursorColor(WHITE), selectionColor({100,100,100,50}) {
    font = FontsHandler::get("default");
}

static float getRealCursorPos(unsigned int cursorPos, Font* font, const char *text) {
    return fh::MeasureTextWidth(font, TextSubtext(text, 0, cursorPos))+1;
}

Meatball::InputTextBox::InputTextBox()
 : rect({0,0,0,0}) {}

Meatball::InputTextBox::InputTextBox(const Rectangle &rect)
 : rect(rect) {}

void Meatball::InputTextBox::draw() {
    BeginScissorMode(rect.x-1, rect.y, rect.width, rect.height);

    drawText(config->font, text,
        rect.x-offsetX,
        rect.y+rect.height/2-config->font->baseSize/2+1/*+1 because it sticks 1 pixel on top*/,
        config->textColor);
    
    // maybe cursor should be outside inputtextbox and be a function or class too like borders and stuff
    if (focused) {
        float x = rect.x-offsetX+getRealCursorPos(cursorPos, config->font, text);
        DrawLine(x, rect.y, x, rect.y+rect.height, config->cursorColor);
        
        if (selectedTextStartIdx != getTextMaxSize() && selectedTextStartIdx != selectedTextFinalIdx) {
            float selectedX, selectedWidth;
            
            if (selectedTextFinalIdx > selectedTextStartIdx) {
                selectedX = getRealCursorPos(selectedTextStartIdx, config->font, text);
                selectedWidth = getRealCursorPos(selectedTextFinalIdx, config->font, text)-selectedX;
            } else {
                selectedX = getRealCursorPos(selectedTextFinalIdx, config->font, text);
                selectedWidth = getRealCursorPos(selectedTextStartIdx, config->font, text)-selectedX;
            }
            
            DrawRectangle(rect.x-offsetX+selectedX, rect.y, selectedWidth, rect.height, config->selectionColor);
        }
    }

    EndScissorMode();
}

void Meatball::InputTextBox::update() {
    unsigned short textSize = strlen(text);
    unsigned short textMaxSize = getTextMaxSize();
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        bool wasFocusedBefore = focused;
        focused = CheckCollisionPointRec(GetMousePosition(), rect);
        
        if (wasFocusedBefore && focused && strlen(text) != 0) {
            unsigned int newCursorPos = 0;
            float textWidth = 0;
            
            while (newCursorPos < strlen(text) && textWidth < GetMouseX()-rect.x+offsetX) {
                textWidth += fh::MeasureTextWidth(config->font, TextSubtext(text, newCursorPos, 1))+1;
                ++newCursorPos;
            }
            
            cursorPos = newCursorPos;
            selectedTextStartIdx = selectedTextFinalIdx = textMaxSize;
            mousePressed = true;
        }
    }

    if (mousePressed) {
        if (strlen(text) != 0 && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (selectedTextStartIdx == textMaxSize)
                selectedTextStartIdx = cursorPos;
            
            float newCursorPos = 0;
            float textWidth = 0;
            
            while (newCursorPos < strlen(text) && textWidth < GetMouseX()-rect.x+offsetX) {
                textWidth += fh::MeasureTextWidth(config->font, TextSubtext(text, newCursorPos, 1))+1;
                ++newCursorPos;
            }
            
            cursorPos = newCursorPos;
            selectedTextFinalIdx = cursorPos;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            mousePressed = false;
    }

    if (!focused) return;

    bool selectMode = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
    bool moveMode = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);

    if (moveMode) {
        if (IsKeyPressed(KEY_A) && strlen(text) != 0) {
            selectedTextStartIdx = 0;
            cursorPos = strlen(text);
            selectedTextFinalIdx = cursorPos;
        }

        if (IsKeyPressed(KEY_C) && selectedTextFinalIdx != textMaxSize)
            SetClipboardText(TextSubtext(text, selectedTextFinalIdx > selectedTextStartIdx? selectedTextStartIdx : selectedTextFinalIdx, selectedTextFinalIdx > selectedTextStartIdx? selectedTextFinalIdx-selectedTextStartIdx
             : selectedTextStartIdx-selectedTextFinalIdx));
        
        else if ((IsKeyPressed(KEY_V) || IsKeyPressedRepeat(KEY_V)) && textSize < textMaxSize) {
            if (selectedTextFinalIdx <= strlen(text)) {
                // cursorPos = left idx
                cursorPos = selectedTextFinalIdx > selectedTextStartIdx? selectedTextStartIdx : selectedTextFinalIdx;
                // cursorPos+delta = right idx
                unsigned short delta = selectedTextFinalIdx > selectedTextStartIdx? selectedTextFinalIdx-selectedTextStartIdx
                : selectedTextStartIdx-selectedTextFinalIdx;
                
                // erase selected text
                textErase(text, cursorPos, delta);
                textSize = strlen(text);

                selectedTextStartIdx = selectedTextFinalIdx = textMaxSize;
            }

            const char *clipboard = GetClipboardText();
            cursorPos = textInsert(text, textMaxSize, cursorPos, clipboard);
            textSize = strlen(text);
        }
    }
    
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT)) {
        if (selectMode && selectedTextStartIdx == textMaxSize)
            selectedTextStartIdx = cursorPos;

        if (moveMode) {
            while (cursorPos != 0 && text[cursorPos-1] == ' ') --cursorPos;
            while (cursorPos != 0 && text[cursorPos-1] != ' ') --cursorPos;
        } else if (!selectMode && selectedTextFinalIdx != textMaxSize) {
            cursorPos = selectedTextFinalIdx > selectedTextStartIdx? selectedTextStartIdx : selectedTextFinalIdx;

        } else if (cursorPos != 0) --cursorPos;
        
        if (selectMode)
            selectedTextFinalIdx = cursorPos;
        else
            selectedTextStartIdx = selectedTextFinalIdx = textMaxSize;
    
    } else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT)) {
        if (selectMode && selectedTextStartIdx == textMaxSize)
            selectedTextStartIdx = cursorPos;
        
        if (moveMode) {
            while (cursorPos != textSize && text[cursorPos] == ' ') ++cursorPos;
            while (cursorPos != textSize && text[cursorPos] != ' ') ++cursorPos;
        } else if (!selectMode && selectedTextFinalIdx != textMaxSize) {
            cursorPos = selectedTextFinalIdx > selectedTextStartIdx? selectedTextFinalIdx : selectedTextStartIdx;

        } else if (cursorPos != textSize) ++cursorPos;
        
        if (selectMode)
            selectedTextFinalIdx = cursorPos;
        else {
            selectedTextStartIdx = textMaxSize;
            selectedTextFinalIdx = textMaxSize;
        }
    
    } else if ((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) && cursorPos != 0) {
        if (selectedTextFinalIdx != textMaxSize) {
            cursorPos = selectedTextFinalIdx > selectedTextStartIdx? selectedTextStartIdx : selectedTextFinalIdx;
            size_t delta = selectedTextFinalIdx > selectedTextStartIdx? selectedTextFinalIdx-selectedTextStartIdx
             : selectedTextStartIdx-selectedTextFinalIdx;
            
            textErase(text, cursorPos, delta);
            textSize = strlen(text);
            
            selectedTextStartIdx = selectedTextFinalIdx = textMaxSize;
        
        } else if (moveMode) {
            // if already erased space, to not erase other chars
            bool usedSpaceVersion = false;
            
            while (cursorPos != 0 && text[cursorPos-1] == ' ') {
                textErase(text, cursorPos-1, 1);
                --cursorPos;
                usedSpaceVersion = true;
            }
            
            if (!usedSpaceVersion) while (cursorPos != 0 && text[cursorPos-1] != ' ') {
                textErase(text, cursorPos-1, 1);
                --cursorPos;
            }

            textSize = strlen(text);
        
        } else {
            textErase(text, cursorPos-1, 1);
            --cursorPos;
            textSize = strlen(text);
        }
        
        if (onTextChange) onTextChange(text);
    
    } else if ((IsKeyPressed(KEY_DELETE) || IsKeyPressedRepeat(KEY_DELETE)) && cursorPos != textSize) {
        if (selectedTextFinalIdx != textMaxSize)
            selectedTextStartIdx = selectedTextFinalIdx = textMaxSize;
        
        if (moveMode) {
            // if already erased space, to not erase other chars
            bool usedSpaceVersion = false;
            
            while (cursorPos != textSize && text[cursorPos] == ' ') {
                textErase(text, cursorPos, 1);
                usedSpaceVersion = true;
            }
            
            if (!usedSpaceVersion) while (cursorPos != textSize && text[cursorPos] != ' ')
                textErase(text, cursorPos, 1);
            
            textSize = strlen(text);
        } else {
           textErase(text, cursorPos, 1);
            textSize = strlen(text);
        }
        
        if (onTextChange) onTextChange(text);
    }
    
    int codePoint = 0;
    if (codePoint = GetCharPressed(), codePoint) {
        if (selectedTextFinalIdx != textMaxSize) {
            cursorPos = selectedTextFinalIdx > selectedTextStartIdx? selectedTextStartIdx : selectedTextFinalIdx;
            size_t delta = selectedTextFinalIdx > selectedTextStartIdx? selectedTextFinalIdx-selectedTextStartIdx
             : selectedTextStartIdx-selectedTextFinalIdx;
            
            textErase(text, cursorPos, delta);
            textSize = strlen(text);
        }

        const char* c = (const char*)&codePoint;
        textInsert(text, textMaxSize, cursorPos, c);
        if (onTextChange) onTextChange(text);
        
        ++cursorPos;

        selectedTextStartIdx = selectedTextFinalIdx = textMaxSize;
    }

    if (onSend && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) && textSize != 0) {
        onSend(text);
        text[0] = '\0';
        cursorPos = textSize;
        offsetX = 0;
        selectedTextStartIdx = selectedTextFinalIdx = textMaxSize;
    
    } else {
        float x = getRealCursorPos(cursorPos, config->font, text);
        if (x-offsetX > rect.width || x-offsetX < 0 || offsetX > rect.width) {
            offsetX = x-rect.width+1;
            if (offsetX < 0) offsetX = 0;
        }
    }
}

const char* Meatball::InputTextBox::getText() {
    return text;
}