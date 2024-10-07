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
    /// @return hovered
    bool handleRectHover(bool hovered, const Vector2& mousePosition, const Rectangle& rect, const VoidFunc& onHover, const VoidFunc& onUnhover);

    /// @note it sets font texture filter to bilinear
    /// @return false if path does not exist or is directory
    bool loadFont(const std::filesystem::path& path, int size, int* codepoints, int codepointCount, Font& outputFont);

    Vector2 measureText(const Font& font, float height, const char* text);
        
    float measureTextWidth(const Font& font, float height, const char* text);
        
    float measureTextHeight(const Font& font, float height, const char* text);

    bool parseStringToColor(const std::string& str, Color& buf);
}

bool operator==(const Color& left, const Color& right);