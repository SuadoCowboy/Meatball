#include "DrawFuncs.h"

void Meatball::drawRect(const Rectangle& rect, const Color& color) {
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, color);
}

void Meatball::drawX(const Rectangle& rect, const Color& color) {
    /*
    .
     .
      .
    */
    DrawLine(rect.x,rect.y, rect.x+rect.width-1,rect.y+rect.height-1, color);
    /*
      .
     .
    .
    */
    DrawLine(rect.x,rect.y+rect.height-1, rect.x+rect.width-1,rect.y, color);
}

void Meatball::drawText(const Font& font, float height, const char* text, float x, float y, const Color& color) {
    SetTextLineSpacing(height);
    DrawTextEx(font, text, {x, y}, height, textSpacing, color);
}