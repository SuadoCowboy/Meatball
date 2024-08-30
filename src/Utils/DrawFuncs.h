#pragma once

#include <raylib.h>

#include "Utils.h"

namespace Meatball {
    void drawRect(const Rectangle& rect, const Color& color);

    /// @brief draws a X inside the rect bounds
    void drawX(Rectangle& rect, Color& color);

    /// @brief draws text inside the button bounds
    void drawText(const Font& font, float height, const char* text, float x, float y, const Color& color);
}