#include "Utils.h"

#include <cstring>

void Meatball::checkHovered(bool &hovered, const Rectangle &rect, VoidFunc *onHover, VoidFunc *onRelease) {
    bool wasHovered = hovered;
    hovered = CheckCollisionPointRec(GetMousePosition(), rect);
    
    if (!wasHovered && hovered && onHover && *onHover)
        (*onHover)();
    
    if (hovered && onRelease && *onRelease && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        (*onRelease)();
}

float Meatball::getContentHeight(float fontSize, const std::list<std::string> &text) {
    size_t lineIdx = 0;
    for (auto &line : text) {
        size_t newLineIdx = 0;
        while (newLineIdx != std::string::npos) {
            newLineIdx = line.find('\n', newLineIdx+1);
            lineIdx++;
        }
    }

    return lineIdx*fontSize; // contentHeight
}

Vector2 Meatball::getRectCenter(const Rectangle &rect) {
    return {rect.x+rect.width*0.5f, rect.y+rect.height*0.5f};
}

float Meatball::getRectCenterX(const Rectangle &rect) {
    return rect.x+rect.width*0.5;
}

float Meatball::getRectCenterY(const Rectangle &rect) {
    return rect.y+rect.height*0.5;
}

void Meatball::fitXYInRenderScreen(Rectangle &rect, const Vector2 &minPos, const Vector2 &maxPos) {
    if (rect.x < minPos.x) rect.x = minPos.x;
    else if (rect.x > GetRenderWidth()+maxPos.x) rect.x = GetRenderWidth()-maxPos.x;
    
    if (rect.y < minPos.y) rect.y = minPos.y;
    else if (rect.y > GetRenderHeight()+maxPos.y) rect.y = GetRenderHeight()-maxPos.y;
}

void Meatball::textErase(char* text, size_t index, size_t length) {
    if (strlen(text) < index+length) return;
    
    while (true) {
		text[index] = text[index+length];
		if (text[index+length] == '\0') break;
		++index;
	}
}

size_t Meatball::textInsert(char* dest, size_t destMaxSize, size_t destIdx, const char* source) {
    size_t sourceIdx = 0;
    size_t destSize = strlen(dest);
	
	while (source[sourceIdx] != '\0' && destSize != destMaxSize) {
		char charBefore = dest[destIdx];
		dest[destIdx] = source[sourceIdx];
		++destSize;
	
		for (size_t i = destIdx+1; i < destSize; ++i) {
			char actualChar = dest[i];
			dest[i] = charBefore;
			charBefore = actualChar;
		}

		++destIdx;
		++sourceIdx;
	}

	dest[destSize] = '\0';

    return destIdx;
}

bool operator==(const Color &left, const Color &right) {
    return (left.r == right.r) && 
     (left.g == right.g) && 
     (left.b == right.b) && 
     (left.a == right.a);
}