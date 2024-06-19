#include "MouseCursor.h"

MouseCursor Meatball::currentMouseCursor = MOUSE_CURSOR_DEFAULT;
unsigned char Meatball::currentMouseCursorPriorityLevel = 0;

void Meatball::setCursor(MouseCursor cursor, unsigned char priorityLevel) {
    if (priorityLevel < currentMouseCursorPriorityLevel)
        return;
    
    SetMouseCursor(cursor);
    currentMouseCursor = cursor;
    currentMouseCursorPriorityLevel = priorityLevel;
}

void Meatball::resetCursor(unsigned char priorityLevel) {
    if (currentMouseCursorPriorityLevel != priorityLevel) return;

    currentMouseCursor = MOUSE_CURSOR_DEFAULT;
    currentMouseCursorPriorityLevel = 0;
    
    SetMouseCursor(currentMouseCursor);
}