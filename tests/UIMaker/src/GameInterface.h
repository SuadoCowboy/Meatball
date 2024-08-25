#pragma once

#include <functional>

#include <HayBCMD.h>
#include <Console.h>
#include <Interface/Button.h>
#include <Interface/DynamicPanel.h>

#include <raylib.h>

#define UI_TYPE_BUTTON 0
#define UI_TYPE_DYNAMIC_PANEL 1

struct UIObject {
    void* object = nullptr;
    const char* name;
    unsigned char type;
    void (*update)(void*);
    void (*draw)(void*);

    UIObject(void* object, const char* name, unsigned char type,
            void (*update)(void*), void (*draw)(void*))
        : object(object), name(name), type(type), update(update), draw(draw) {}
    
    ~UIObject() {
        switch (type) {
            case UI_TYPE_BUTTON:
                delete (Meatball::Button*)object;
                break;
            case UI_TYPE_DYNAMIC_PANEL:
                delete (Meatball::DynamicPanel*)object;
                break;
        }
    }
};

struct UIOption {
    const char* text;
    Font& font;
    unsigned char type;

    UIOption(const char* text, Font& font, unsigned char type) : text(text), font(font), type(type) {}
};

UIObject* createUIObject(const char* name, std::vector<UIObject*>& uiObjects, const Vector2& position, unsigned char type, const Vector2& renderSize);