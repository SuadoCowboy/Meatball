#include "DynamicPanel.h"

#include "Utils/Utils.h"

Meatball::DynamicPanel::DynamicPanel(std::shared_ptr<Config::DynamicPanel> config)
 : config(config), rect({0,0,0,0}) {}

Meatball::DynamicPanel::DynamicPanel(const Rectangle &rect, std::shared_ptr<Config::DynamicPanel> config)
 : config(config), rect(rect) {}

void Meatball::DynamicPanel::update() {
    Vector2 mousePos = GetMousePosition();
    
    /*
    conditions:
    1 = dragging
    2 = resizing
    4 = resizingFromN
    8 = resizingFromW
    16 = hovered
    */

    if ((!(conditions & 1) && !(conditions & 2) && !CheckCollisionPointRec(mousePos, {rect.x, rect.y, rect.width, rect.height})) || !IsWindowFocused()) {
        if (conditions & 16) {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT); // what if two dynamic panels intersects? TODO: test this case
            conditions &= ~16;
        }
        return;
    }
    
    conditions |= 16;

    if (!(conditions & 1) && !(conditions & 2)) {
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    
    if (CheckCollisionPointRec(mousePos, {rect.x+2, rect.y+2, rect.width-4, config->grabHeight})) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            offset = {GetMouseX()-rect.x, GetMouseY()-rect.y};
            conditions |= 1;
        }
    }

    // Resize North
    else if (CheckCollisionPointRec(mousePos, {rect.x+2, rect.y, rect.width-4, 2})) {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            offset = {-1, GetMouseY()-rect.y};
            conditions |= 2;
            conditions |= 4;
        }
    }

    // Resize West 
    else if (CheckCollisionPointRec(mousePos, {rect.x, rect.y+2, 2, rect.height-4})) {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            offset = {GetMouseX()-rect.x, -1};
            conditions |= 2;
            conditions |= 8;
        }
    }

    // Resize South
    else if (CheckCollisionPointRec(mousePos, {rect.x+2, rect.y+rect.height-2, rect.width-4, 2})) {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            offset = {-1, GetMouseY()-rect.height};
            conditions |= 2;
        }
    }

    // Resize East
    else if (CheckCollisionPointRec(mousePos, {rect.x+rect.width-2, rect.y+2, 2, rect.height-4})) {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            offset = {GetMouseX()-rect.width, -1};
            conditions |= 2;
        }
    }

    // Resize South West
    else if (CheckCollisionPointRec(mousePos, {rect.x-1, rect.y+rect.height-2, 3, 3})) {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_NESW);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            offset = {GetMouseX()-rect.x, GetMouseY()-rect.height};
            conditions |= 2;
            conditions |= 8;
        }
    }
    
    // Resize North West
    else if (CheckCollisionPointRec(mousePos, {rect.x-1, rect.y-1, 3, 3})) {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_NWSE);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            offset = {GetMouseX()-rect.x, GetMouseY()-rect.y};
            conditions |= 2;
            conditions |= 4;
        }
    }

    // Resize North East
    else if (CheckCollisionPointRec(mousePos, {rect.x+rect.width-2, rect.y-1, 3, 3})) {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_NESW);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            offset = {GetMouseX()-rect.width, GetMouseY()-rect.y};
            conditions |= 2;
            conditions |= 4;
        }
    }

    // Resize South East
    else if (CheckCollisionPointRec(mousePos, {rect.x+rect.width-2, rect.y+rect.height-2, 3, 3})) {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_NWSE);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            offset = {GetMouseX()-rect.width, GetMouseY()-rect.height};
            conditions |= 2;
        }
    }
    } // if (!resizing && !dragging)

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        conditions &= ~1;
        if (onResizeStop && (conditions & 2)) onResizeStop();
        conditions &= ~2;
        conditions &= ~4;
        conditions &= ~8;
    }

    if (conditions & 1) {
        float oldX = rect.x;
        float oldY = rect.y;
        rect.x = mousePos.x-offset.x;
        rect.y = mousePos.y-offset.y;

        fitXYInRenderScreen(rect, {6-rect.width, 0}, {6, 6});

        if (onMove && (rect.x != oldX || oldY != rect.y)) onMove();
    
    } else if (conditions & 2) {
        float oldX = rect.x;
        float oldY = rect.y;
        float oldWidth = rect.width;
        float oldHeight = rect.height;

        if (offset.x != -1) {
            if (conditions & 8) {
                rect.x = mousePos.x-offset.x;
                
                if (rect.width-rect.x+oldX > config->minSize.x)
                    rect.width -= rect.x-oldX;
                else {
                    rect.x = oldX+rect.width-config->minSize.x;
                    rect.width = config->minSize.x;
                }
            
            } else rect.width = mousePos.x-offset.x;
        }
        
        if (offset.y != -1) {
            if (conditions & 4) {
                rect.y = mousePos.y-offset.y;
                
                if (rect.height-rect.y+oldY > config->minSize.y)
                    rect.height -= rect.y-oldY;
                else {
                    rect.y = oldY+rect.height-config->minSize.y;
                    rect.height = config->minSize.y;
                }
            
            } else rect.height = mousePos.y-offset.y;
        }

        if (rect.width < config->minSize.x) rect.width = config->minSize.x;
        if (rect.height < config->minSize.y) rect.height = config->minSize.y;

        fitXYInRenderScreen(rect, {6-rect.width, 6}, {6, 6});

        if (onResize && (oldWidth != rect.width || oldHeight != rect.height)) onResize(); // onResize should also call onMove
        else if (onMove && (oldX != rect.x || oldY != rect.y)) onMove();
    }
}

bool Meatball::DynamicPanel::isAnyConditionActive() {
    return conditions == 0? false : true;
}