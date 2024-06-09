#pragma once

#include <raylib.h>

#include "Shared.h"

namespace Meatball {
    /// @brief since alot of classes might do hover check, this function exists to facilitate the process of implementing it
    void checkHovered(bool &hovered, const Rectangle &rect, VoidFunc *onHover, VoidFunc *onRelease);
    
    Vector2 getRectCenter(const Rectangle &rect);
    float getRectCenterX(const Rectangle &rect);
    float getRectCenterY(const Rectangle &rect);

    /// @brief modifies the rect position to fit in the render screen
    /// @param maxPos the max pos is already counted with render position so you don't need to sum it yourself
    void fitXYInRenderScreen(Rectangle &rect, const Vector2 &minPos, const Vector2 &maxPos);
}

bool operator==(const Color &left, const Color &right);