#include "CvarFuncs.h"

void Meatball::CVARFuncs::setColor(void *pData, const std::string& str) {
    parseStringToColor(str, *(Color*)pData);
}

std::string Meatball::CVARFuncs::getColor(void *pData) {
    Color color = *(Color*)pData;
    return SweatCI::formatString("{}, {}, {}, {}", (short)color.r, (short)color.g, (short)color.b, (short)color.a);
}