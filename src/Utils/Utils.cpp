#include "Utils.h"

#include "FontsHandler.h"

float Meatball::textSpacing = 1.0f;

void Meatball::checkHovered(bool &hovered, const Rectangle &rect, VoidFunc *onHover, VoidFunc *onRelease) {
    bool wasHovered = hovered;
    hovered = CheckCollisionPointRec(GetMousePosition(), rect);
    
    if (!wasHovered && hovered && onHover && *onHover)
        (*onHover)();
    
    if (hovered && onRelease && *onRelease && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        (*onRelease)();
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

void Meatball::fitXYInRenderScreen(float &x, float &y, const Vector2 &minPos, const Vector2 &maxPos) {
    if (x < minPos.x) x = minPos.x;
    else if (x > GetRenderWidth()-maxPos.x) x = GetRenderWidth()-maxPos.x;
    
    if (y < minPos.y) y = minPos.y;
    else if (y > GetRenderHeight()-maxPos.y) y = GetRenderHeight()-maxPos.y;
}

bool Meatball::loadFont(const std::filesystem::path& path, unsigned short fontsHandlerId, int size, int *codepoints, int codepointCount) {
    if (!std::filesystem::exists(path) || std::filesystem::is_directory(path))
        return false;
    
    Font font = LoadFontEx(path.string().c_str(), size, codepoints, codepointCount);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    FontsHandler::add(fontsHandlerId, font);
    return true;
}

Vector2 Meatball::measureText(const Font &font, float height, const char *text) {
    SetTextLineSpacing(height);
    return MeasureTextEx(font, text, height, textSpacing);
}
    
float Meatball::measureTextWidth(const Font &font, float height, const char *text) {
    SetTextLineSpacing(height);
    Vector2 textSize = MeasureTextEx(font, text, height, textSpacing);
    return textSize.x;
}
    
float Meatball::measureTextHeight(const Font &font, float height, const char *text) {
    SetTextLineSpacing(height);
    Vector2 textSize = MeasureTextEx(font, text, height, textSpacing);
    return textSize.y;
}

bool operator==(const Color &left, const Color &right) {
    return (left.r == right.r) && 
     (left.g == right.g) && 
     (left.b == right.b) && 
     (left.a == right.a);
}