#include "GameInterface.h"

#include <Utils/DrawFuncs.h>

void updateButton(void* voidButton) {
    Meatball::Button* button = (Meatball::Button*)voidButton;
    button->update();
}

void drawButton(void* voidButton) {
    Meatball::Button* button = (Meatball::Button*)voidButton;
    Meatball::drawRect(button->rect, button->isHovered()? button->config->hoveredColor : button->config->color);
}

UIObject* createUIObject(const char* name, std::vector<UIObject*>& uiObjects, const Vector2& position, unsigned char type, const Vector2& renderSize) {
    switch (type) {
    case UI_TYPE_BUTTON: {
        Meatball::Button* button = new Meatball::Button((Rectangle){position.x, position.y, renderSize.x*0.1f, renderSize.y*0.06f});
        button->config = &Meatball::Defaults::buttonConfig;

        UIObject* object = new UIObject(
                button,
                name,
                type,
                updateButton,
                drawButton
            );

        uiObjects.push_back(object);
        return object;
        break;
    }
    };

    return nullptr;
}