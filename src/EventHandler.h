#pragma once

#include <functional>

#include <raylib.h>

namespace Meatball::EventHandler {
    typedef std::function<void(int key)> EventFunc;

    extern EventFunc onCharPress;

    extern std::function<void(int key, bool isRepeat)> onKeyboardPress;
    extern EventFunc onKeyboardRelease;

    extern EventFunc onMousePress;
    extern EventFunc onMouseRelease;
    
    extern std::function<void(const Vector2& dir)> onMouseWheel;

    extern std::function<void(const Vector2& delta, const Vector2& position)> onMouseMove;

    //extern EventFunc> onGamepadPress;
    //extern EventFunc> onGamepadRelease;

    void handle();
}