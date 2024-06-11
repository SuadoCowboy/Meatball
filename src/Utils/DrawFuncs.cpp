#include "DrawFuncs.h"

void Meatball::drawRect(const Rectangle &rect, const Color &color) {
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, color);
}

void Meatball::drawX(Rectangle &rect, Color &color) {
    /*
    .
     .
      .
    */
    DrawLine(rect.x,rect.y, rect.x+rect.width,rect.y+rect.height, color);
    /*
      .
     .
    .
    */
    DrawLine(rect.x,rect.y+rect.height, rect.x+rect.width,rect.y, color);
}

void Meatball::drawText(const Font &font, const char *text, float x, float y, Color color) {
        //SetTextLineSpacing(textSpacing); // is this required?
        DrawTextEx(font, text, {x, y}, font.baseSize, textSpacing, color);
    }