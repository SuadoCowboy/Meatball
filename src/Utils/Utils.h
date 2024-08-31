#pragma once

#include <filesystem>

#include <raylib.h>

#include "../Shared.h"

namespace Meatball {
    // because Raylib does not provide a textSpacing global, I'm doing my own
    extern float textSpacing;

    struct ColoredText {
        ColoredText(const std::string& text, const Color& color);

        std::string text;
        Color color;
    };

    /// @brief since alot of classes might do hover check, this function exists to facilitate the process of implementing it
    void checkHovered(bool& hovered, const Rectangle& rect, VoidFunc* onHover, VoidFunc* onRelease);
    
    Vector2 getRectCenter(const Rectangle& rect);
    float getRectCenterX(const Rectangle& rect);
    float getRectCenterY(const Rectangle& rect);

    /// @brief modifies the rect position to fit in the render screen
    /// @param maxPos the max pos is already counted with render position so you don't need to sum it yourself
    void fitXYInRenderScreen(float& x, float& y, const Vector2& minPos, const Vector2& maxPos);

    /// @note it sets font texture filter to bilinear
    /// @return false if path does not exist or is directory
    bool loadFont(const std::filesystem::path& path, int size, int* codepoints, int codepointCount, Font& outputFont);

    Vector2 measureText(const Font& font, float height, const char* text);
        
    float measureTextWidth(const Font& font, float height, const char* text);
        
    float measureTextHeight(const Font& font, float height, const char* text);

    bool parseStringToColor(const std::string& str, Color& buf);
}

bool operator==(const Color& left, const Color& right);