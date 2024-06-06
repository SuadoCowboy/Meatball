#include "DrawFuncs.h"

void Meatball::drawRect(Rectangle &rect, Color &color) {
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

void Meatball::drawText(Font *font, const char *text, float x, float y, Color color, float spacing) {
    //SetTextLineSpacing(font->baseSize); // is this required?
    DrawTextEx(*font, text, {x, y}, font->baseSize, spacing, color);
}