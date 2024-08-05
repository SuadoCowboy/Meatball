#pragma once

#include <functional>

#include <HayBCMD.h>
#include <Console.h>
#include <Interface/Button.h>
#include <Interface/DynamicPanel.h>

#include <raylib.h>

#define UI_TYPE_BUTTON 0
#define UI_TYPE_DYNAMIC_PANEL 1

#pragma region macros

#define CREATE_TOGGLE_CVAR(name, boolVar, description) \
    HayBCMD::CVARStorage::setCvar( \
        name, \
        [&](const std::string& s) { \
            int i = 0; \
            if (!stringToInt(s, i)) return; \
            boolVar = (bool)i; \
        }, \
        [&]() { \
            if (boolVar) \
                return std::string("1"); \
            else \
                return std::string("0"); \
        }, description)

#define CREATE_TOGGLE_CVAR_BITWISE(name, flags, bitToToggle, description) \
    HayBCMD::CVARStorage::setCvar( \
        name, \
        [&flags](const std::string& s) { \
            int i = 0; \
            if (!stringToInt(s, i)) return; \
            if (i == 0) \
                flags &= ~bitToToggle; \
            else \
                flags |= bitToToggle; \
        }, \
        [&flags](){ \
            return std::to_string(flags & bitToToggle); \
        }, description)

#pragma endregion

struct UIObject {
    void* object = nullptr;
    const char* name;
    unsigned char type;
    std::function<void()> update, draw;

    UIObject(void* object, const char* name, unsigned char type,
            const std::function<void()>& update, const std::function<void()>& draw)
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

void createUIObject(std::vector<UIObject*>& uiObjects, const Vector2& optionsPosition, unsigned char type, const Vector2& renderSize);