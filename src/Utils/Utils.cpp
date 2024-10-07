#include "Utils.h"

float Meatball::textSpacing = 1.0f;

Meatball::ColoredText::ColoredText(const std::string& text, const Color& color) : text(text), color(color) {}

bool Meatball::handleRectHover(bool hovered, const Vector2& mousePosition, const Rectangle& rect, const VoidFunc& onHover, const VoidFunc& onUnhover) {
    bool wasHovered = hovered;
    hovered = CheckCollisionPointRec(mousePosition, rect);
    
    if (!wasHovered && hovered && onHover)
        (onHover)();
    
    if (hovered && onUnhover)
        (onUnhover)();

    return hovered;
}

bool Meatball::loadFont(const std::filesystem::path& path, int size, int* codepoints, int codepointCount, Font& outputFont) {
    if (!std::filesystem::exists(path) || std::filesystem::is_directory(path))
        return false;
    
    outputFont = LoadFontEx(path.string().c_str(), size, codepoints, codepointCount);
    SetTextureFilter(outputFont.texture, TEXTURE_FILTER_BILINEAR);
    return true;
}

Vector2 Meatball::measureText(const Font& font, float height, const char* text) {
    SetTextLineSpacing(height);
    return MeasureTextEx(font, text, height, textSpacing);
}
    
float Meatball::measureTextWidth(const Font& font, float height, const char* text) {
    SetTextLineSpacing(height);
    Vector2 textSize = MeasureTextEx(font, text, height, textSpacing);
    return textSize.x;
}

float Meatball::measureTextHeight(const Font& font, float height, const char* text) {
    SetTextLineSpacing(height);
    Vector2 textSize = MeasureTextEx(font, text, height, textSpacing);
    return textSize.y;
}

bool Meatball::parseStringToColor(const std::string& str, Color& buf) {
    Color color = BLACK;
    
    try {
        size_t secondCommaIdx;
        {
            size_t firstCommaIdx = str.find(",");
            color.r = std::stoi(str.substr(0, firstCommaIdx));
            
            secondCommaIdx = str.find(",", firstCommaIdx+1);
            color.g = std::stoi(str.substr(firstCommaIdx+1, secondCommaIdx-firstCommaIdx-1));
        }

        size_t thirdCommaIdx = str.find(",", secondCommaIdx+1);

        color.b = std::stoi(str.substr(secondCommaIdx+1, thirdCommaIdx-secondCommaIdx-1));
        if (thirdCommaIdx != std::string::npos)
            color.a = std::stoi(str.substr(thirdCommaIdx+1));
        
        buf = color;
        return true;
    } catch (...) {
        return false;
    }
}

bool operator==(const Color& left, const Color& right) {
    return (left.r == right.r) && 
     (left.g == right.g) && 
     (left.b == right.b) && 
     (left.a == right.a);
}