#pragma once

#include <raylib.h>

namespace Meatball {
    enum MouseCursorPriorityLevel { // highest number = better priority
        DYNAMIC_PANEL = 0,
        BUTTON,
        SCROLL_BAR,
        INPUT_TEXT_BOX,
    };

    void setCursor(MouseCursor cursor, unsigned char priorityLevel);
    void resetCursor(unsigned char priorityLevel);

    extern MouseCursor currentMouseCursor;
    extern unsigned char currentMouseCursorPriorityLevel;
}