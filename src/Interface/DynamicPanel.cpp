#include "DynamicPanel.h"

#include "Utils/Utils.h"
#include "MouseCursor.h"

Meatball::DynamicPanel::DynamicPanel(const Rectangle& rect)
 : rect(rect) {}

void Meatball::DynamicPanel::onMousePress(int button) {
    Vector2 mousePosition = GetMousePosition();

    if ((conditionFlags & 1) || (conditionFlags & 2) || button != MOUSE_BUTTON_LEFT)
        return;
    
    // Grab Window
    if (CheckCollisionPointRec(mousePosition, {rect.x+2, rect.y+2, rect.width-4, grabHeight})) {
        offset = {mousePosition.x-rect.x, mousePosition.y-rect.y};
        conditionFlags |= 1;
    }

    // Resize North
    else if (CheckCollisionPointRec(mousePosition, {rect.x+2, rect.y, rect.width-4, 2})) {
        offset = {-1, mousePosition.y-rect.y};
        conditionFlags |= 2;
        conditionFlags |= 4;
    }

    // Resize West 
    else if (CheckCollisionPointRec(mousePosition, {rect.x, rect.y+2, 2, rect.height-4})) {
        setCursor(MOUSE_CURSOR_RESIZE_EW, MouseCursorPriorityLevel::DYNAMIC_PANEL);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            offset = {mousePosition.x-rect.x, -1};
            conditionFlags |= 2;
            conditionFlags |= 8;
        }
    }

    // Resize South
    else if (CheckCollisionPointRec(mousePosition, {rect.x+2, rect.y+rect.height-2, rect.width-4, 2})) {
        offset = {-1, mousePosition.y-rect.height};
        conditionFlags |= 2;
    }

    // Resize East
    else if (CheckCollisionPointRec(mousePosition, {rect.x+rect.width-2, rect.y+2, 2, rect.height-4})) {
        offset = {mousePosition.x-rect.width, -1};
        conditionFlags |= 2;
    }

    // Resize South West
    else if (CheckCollisionPointRec(mousePosition, {rect.x-1, rect.y+rect.height-2, 3, 3})) {
        offset = {mousePosition.x-rect.x, mousePosition.y-rect.height};
        conditionFlags |= 2;
        conditionFlags |= 8;
    }
    
    // Resize North West
    else if (CheckCollisionPointRec(mousePosition, {rect.x-1, rect.y-1, 3, 3})) {
        offset = {mousePosition.x-rect.x, mousePosition.y-rect.y};
        conditionFlags |= 2;
        conditionFlags |= 4;
        conditionFlags |= 8;
    }

    // Resize North East
    else if (CheckCollisionPointRec(mousePosition, {rect.x+rect.width-2, rect.y-1, 3, 3})) {
        offset = {mousePosition.x-rect.width, mousePosition.y-rect.y};
        conditionFlags |= 2;
        conditionFlags |= 4;
    }

    // Resize South East
    else if (CheckCollisionPointRec(mousePosition, {rect.x+rect.width-2, rect.y+rect.height-2, 3, 3})) {
        offset = {mousePosition.x-rect.width, mousePosition.y-rect.height};
        conditionFlags |= 2;
    }
}

void Meatball::DynamicPanel::onMouseRelease(int button) {
    if (button == MOUSE_BUTTON_LEFT) {
        conditionFlags &= ~1;
        if (onResizeStop && (conditionFlags & 2)) onResizeStop();
        conditionFlags &= ~2;
        conditionFlags &= ~4;
        conditionFlags &= ~8;
    }
}

void Meatball::DynamicPanel::onMouseMove(const Vector2& mousePosition) {
    /*
    conditionFlags:
    1 = dragging
    2 = resizing
    4 = resizingFromN
    8 = resizingFromW
    16 = hovered
    */
    if ((!(conditionFlags & 1) && !(conditionFlags & 2) && !CheckCollisionPointRec(mousePosition, {rect.x, rect.y, rect.width, rect.height})) || !IsWindowFocused()) {
        if (conditionFlags & 16) {
            // what if two dynamic panels intersects? TODO: test this case
            resetCursor(MouseCursorPriorityLevel::DYNAMIC_PANEL);
            conditionFlags &= ~16;
        }
        return;
    }
    
    conditionFlags |= 16;

    if (!(conditionFlags & 1) && !(conditionFlags & 2)) {
        if (CheckCollisionPointRec(mousePosition, {rect.x+2, rect.y+2, rect.width-4, grabHeight}))
            setCursor(MOUSE_CURSOR_POINTING_HAND, MouseCursorPriorityLevel::DYNAMIC_PANEL);
        
        else if (CheckCollisionPointRec(mousePosition, {rect.x+2, rect.y, rect.width-4, 2}))
            setCursor(MOUSE_CURSOR_RESIZE_NS, MouseCursorPriorityLevel::DYNAMIC_PANEL);
        
        else if (CheckCollisionPointRec(mousePosition, {rect.x, rect.y+2, 2, rect.height-4}))
            setCursor(MOUSE_CURSOR_RESIZE_EW, MouseCursorPriorityLevel::DYNAMIC_PANEL);
        
        else if (CheckCollisionPointRec(mousePosition, {rect.x+2, rect.y+rect.height-2, rect.width-4, 2}))
            setCursor(MOUSE_CURSOR_RESIZE_NS, MouseCursorPriorityLevel::DYNAMIC_PANEL);
        
        else if (CheckCollisionPointRec(mousePosition, {rect.x+rect.width-2, rect.y+2, 2, rect.height-4}))
            setCursor(MOUSE_CURSOR_RESIZE_EW, MouseCursorPriorityLevel::DYNAMIC_PANEL);
        
        else if (CheckCollisionPointRec(mousePosition, {rect.x-1, rect.y+rect.height-2, 3, 3}))
            setCursor(MOUSE_CURSOR_RESIZE_NESW, MouseCursorPriorityLevel::DYNAMIC_PANEL);
        
        else if (CheckCollisionPointRec(mousePosition, {rect.x-1, rect.y-1, 3, 3}))
            setCursor(MOUSE_CURSOR_RESIZE_NWSE, MouseCursorPriorityLevel::DYNAMIC_PANEL);
        
        else if (CheckCollisionPointRec(mousePosition, {rect.x+rect.width-2, rect.y-1, 3, 3}))
            setCursor(MOUSE_CURSOR_RESIZE_NESW, MouseCursorPriorityLevel::DYNAMIC_PANEL);
        
        else if (CheckCollisionPointRec(mousePosition, {rect.x+rect.width-2, rect.y+rect.height-2, 3, 3}))
            setCursor(MOUSE_CURSOR_RESIZE_NWSE, MouseCursorPriorityLevel::DYNAMIC_PANEL);
        
        else
            resetCursor(MouseCursorPriorityLevel::DYNAMIC_PANEL);
    } // if (!resizing && !dragging)

    if (conditionFlags & 1) {
        float oldX = rect.x;
        float oldY = rect.y;
        rect.x = mousePosition.x-offset.x;
        rect.y = mousePosition.y-offset.y;

        if (onMove && (rect.x != oldX || oldY != rect.y)) onMove();
    
    } else if (conditionFlags & 2) {
        float oldX = rect.x;
        float oldY = rect.y;
        float oldWidth = rect.width;
        float oldHeight = rect.height;

        if (offset.x != -1) {
            if (conditionFlags & 8) {
                rect.x = mousePosition.x-offset.x;
                
                if (rect.width-rect.x+oldX > minSize.x)
                    rect.width -= rect.x-oldX;
                else {
                    rect.x = oldX+rect.width-minSize.x;
                    rect.width = minSize.x;
                }
            
            } else rect.width = mousePosition.x-offset.x;
        }
        
        if (offset.y != -1) {
            if (conditionFlags & 4) {
                rect.y = mousePosition.y-offset.y;

                if (rect.height-rect.y+oldY > minSize.y)
                    rect.height -= rect.y-oldY;
                else {
                    rect.y = oldY+rect.height-minSize.y;
                    rect.height = minSize.y;
                }
            
            } else rect.height = mousePosition.y-offset.y;
        }

        if (rect.width < minSize.x) rect.width = minSize.x;
        if (rect.height < minSize.y) rect.height = minSize.y;

        if (onResize && (oldWidth != rect.width || oldHeight != rect.height)) onResize(); // onResize should also call onMove
        else if (onMove && (oldX != rect.x || oldY != rect.y)) onMove();
    }
}

bool Meatball::DynamicPanel::isAnyConditionActive() {
    return conditionFlags == 0? false : true;
}