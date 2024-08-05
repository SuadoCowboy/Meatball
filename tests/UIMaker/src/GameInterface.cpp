#include "GameInterface.h"

#include <Utils/DrawFuncs.h>

void createUIObject(std::vector<UIObject*>& uiObjects, const Vector2& optionsPosition, unsigned char type, const Vector2& renderSize) {
    switch (type) {
    case UI_TYPE_BUTTON: {
        Meatball::Button* button = new Meatball::Button((Rectangle){optionsPosition.x, optionsPosition.y, renderSize.x*0.1f, renderSize.y*0.06f});
        button->config = std::make_shared<Meatball::Config::Button>(Meatball::Defaults::buttonConfig);

        UIObject* object = new UIObject(
                button,
                HayBCMD::formatString("Item{}", uiObjects.size()).c_str(),
                type,
                [button](){button->update();},
                [button](){
                Meatball::drawRect(button->rect, button->isHovered()? button->config->hoveredColor : button->config->color);
        });

        uiObjects.push_back(object);
        break;
    }
    };
}