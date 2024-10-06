#pragma once

#include <vector>
#include <functional>

namespace Meatball::EventHandler {
    typedef std::function<void(int key)> EventFunc;

    extern std::vector<EventFunc> onKeyboardPressFunctions;
    extern std::vector<EventFunc> onKeyboardReleaseFunctions;

    extern std::vector<EventFunc> onMousePressFunctions;
    extern std::vector<EventFunc> onMouseReleaseFunctions;
    
    extern std::vector<std::function<void(float x, float y)>> onMouseWheelFunctions;

    //extern std::vector<EventFunc> onGamepadPressFunctions;
    //extern std::vector<EventFunc> onGamepadReleaseFunctions;

    void handle();
}