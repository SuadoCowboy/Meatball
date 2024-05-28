#include "DynamicPanel.h"

Meatball::DynamicPanel::DynamicPanel()
 : color(BLACK), rect({0,0,0,0}), offset({0,0}), minSize({1,1}),
 resizing(false), dragging(false), resizingFromN(false), resizingFromW(false) {}

Meatball::DynamicPanel::DynamicPanel(Rectangle rect, Vector2 minSize)
 : rect(rect), offset({0,0}), minSize(minSize),
 resizing(false), dragging(false), resizingFromN(false), resizingFromW(false) {}

Meatball::DynamicPanel::DynamicPanel(float x, float y, float width, float height, Vector2 minSize)
 : offset({0,0}), minSize(minSize),
 resizing(false), dragging(false), resizingFromN(false), resizingFromW(false) {
    rect = {x, y, width, height};
}

void Meatball::DynamicPanel::update() {
    Vector2 mousePos = GetMousePosition();
    if (!dragging && !resizing && !CheckCollisionPointRec(mousePos, {rect.x, rect.y, rect.width, rect.height})) {
        if (wasHovered) {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT); // what if two dynamic panels intersects?
            wasHovered = false;
        }
        return;
    }
    
    wasHovered = true;

    // Dragging and Resizing
    if (!resizing && !dragging) {
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    if (CheckCollisionPointRec(mousePos, {rect.x, rect.y+2, rect.width, 2})) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            offset = {GetMouseX()-rect.x, GetMouseY()-rect.y};
            dragging = true;
        }
    }

    // Resize North
    if (CheckCollisionPointRec(mousePos, {rect.x+2, rect.y, rect.width-4, 2})) {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            offset = {-1, GetMouseY()-rect.y};
            resizing = true;
            resizingFromN = true;
        }
    }

    // Resize West 
    else if (CheckCollisionPointRec(mousePos, {rect.x, rect.y+2, 2, rect.height-4})) {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            offset = {GetMouseX()-rect.x, -1};
            resizing = true;
            resizingFromW = true;
        }
    }

    // Resize South
    else if (CheckCollisionPointRec(mousePos, {rect.x+2, rect.y+rect.height-2, rect.width-4, 2})) {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            offset = {-1, GetMouseY()-rect.height};
            resizing = true;
        }
    }

    // Resize East
    else if (CheckCollisionPointRec(mousePos, {rect.x+rect.width-2, rect.y+2, 2, rect.height-4})) {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            offset = {GetMouseX()-rect.width, -1};
            resizing = true;
        }
    }

    // Resize South West
    else if (CheckCollisionPointRec(mousePos, {rect.x, rect.y+rect.height-2, 2, 2})) {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_NESW);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            offset = {GetMouseX()-rect.x, GetMouseY()-rect.height};
            resizing = true;
            resizingFromW = true;
        }
    }
    
    // Resize North West
    else if (CheckCollisionPointRec(mousePos, {rect.x, rect.y, 2, 2})) {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_NWSE);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            offset = {GetMouseX()-rect.x, GetMouseY()-rect.y};
            resizing = true;
            resizingFromN = resizingFromW = true;
        }
    }

    // Resize North East
    else if (CheckCollisionPointRec(mousePos, {rect.x+rect.width-2, rect.y, 2, 2})) {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_NESW);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            offset = {GetMouseX()-rect.width, GetMouseY()-rect.y};
            resizing = true;
            resizingFromN = true;
        }
    }

    // Resize South East
    else if (CheckCollisionPointRec(mousePos, {rect.x+rect.width-2, rect.y+rect.height-2, 2, 2})) {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_NWSE);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            offset = {GetMouseX()-rect.width, GetMouseY()-rect.height};
            resizing = true;
        }
    }
    } // if (!resizing && !dragging)

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        dragging = false;
        resizing = false;
        resizingFromN = false;
        resizingFromW = false;
    }

    if (dragging) {
        rect.x = mousePos.x-offset.x;
        rect.y = mousePos.y-offset.y;
    
    } else if (resizing) {
        if (offset.x != -1) {
            if (resizingFromW) {
                float oldX = rect.x;
                rect.x = mousePos.x-offset.x;
                if (oldX+rect.width > rect.x)
                    rect.width -= rect.x-oldX;
                else {
                    rect.x = oldX+rect.width-1;
                    rect.width = 1;
                }
            } else
                rect.width = mousePos.x-offset.x;
        }
        
        if (offset.y != -1) {
            if (resizingFromW) {
                float oldY = rect.y;
                rect.y = mousePos.y-offset.y;
                if (oldY+rect.height > rect.y)
                    rect.height -= rect.y-oldY;
                else {
                    rect.y = oldY+rect.height-1;
                    rect.height = 1;
                }
            } else rect.height = mousePos.y-offset.y;
        }

        if (rect.width < minSize.x) rect.width = minSize.x;
        if (rect.height < minSize.y) rect.height = minSize.y;
    }
}

void Meatball::DynamicPanel::draw() {
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, color);
}