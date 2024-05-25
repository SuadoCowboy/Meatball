#pragma once

#include <list>
#include <string>

#include <raylib.h>

#include "Shared.h"

namespace Meatball {
    /// @brief since alot of classes might do hover check, this function exists to facilitate the process of implementing it
    void checkHovered(bool& hovered, const Rectangle& rect, VoidFunc* onHover, VoidFunc* onClick, VoidFunc* onRelease);

    float getContentHeight(const float& viewHeight, const float& fontSize, const std::list<std::string>& text);
}