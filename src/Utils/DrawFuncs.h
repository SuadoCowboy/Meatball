#pragma once

#include <raylib.h>

namespace Meatball {
    void drawRect(const Rectangle &rect, const Color &color);

    /// @brief draws a X inside the rect bounds
    void drawX(Rectangle &rect, Color &color);

    /// @brief draws text inside the button bounds
    void drawText(Font *font, const char *text, float x, float y, Color color, float spacing = 1);
}