#pragma once

#include <list>
#include <string>

#include <raylib.h>

#include "Shared.h"

namespace Meatball {
    /// @brief since alot of classes might do hover check, this function exists to facilitate the process of implementing it
    void checkHovered(bool &hovered, const Rectangle &rect, VoidFunc *onHover, VoidFunc *onRelease);

    float getContentHeight(float fontSize, const std::list<std::string> &text);

    Vector2 getRectCenter(const Rectangle &rect);
    float getRectCenterX(const Rectangle &rect);
    float getRectCenterY(const Rectangle &rect);

    /// @brief modifies the rect position to fit in the render screen
    /// @param maxPos the max pos is already counted with render position so you don't need to sum it yourself
    void fitXYInRenderScreen(Rectangle &rect, const Vector2 &minPos, const Vector2 &maxPos);

    /// @brief erases from index until index+length
    void textErase(char* dest, size_t index, size_t length);

    /// @brief inserts source until dest max size is reached
    /// @note check if your dest is not at it's max len(at least 1 free space required)
    /// @note dest can not be empty
    /// @warning I don't actually know if this is a safe function and probably it's not even efficient
    /// @return index at end of source in dest
    size_t textInsert(char* dest, size_t destMaxSize, size_t index, const char* source);
}

bool operator==(const Color &left, const Color &right);