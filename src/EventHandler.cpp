#include "EventHandler.h"

#include <raylib.h>

std::vector<int> keyboardKeysPressed;
Vector2 mouseWheelMoveBefore = {0.0f,0.0f};
//std::vector<int> Meatball::EventHandler::gamepadButtonsPressed;

std::vector<Meatball::EventHandler::EventFunc> Meatball::EventHandler::onKeyboardPressFunctions;
std::vector<Meatball::EventHandler::EventFunc> Meatball::EventHandler::onKeyboardReleaseFunctions;

std::vector<Meatball::EventHandler::EventFunc> Meatball::EventHandler::onMousePressFunctions;
std::vector<Meatball::EventHandler::EventFunc> Meatball::EventHandler::onMouseReleaseFunctions;

std::vector<std::function<void(float x, float y)>> Meatball::EventHandler::onMouseWheelFunctions;

//std::vector<Meatball::EventHandler::EventFunc> Meatball::EventHandler::onGamepadPressFunctions;
//std::vector<Meatball::EventHandler::EventFunc> Meatball::EventHandler::onGamepadReleaseFunctions;

void Meatball::EventHandler::handle() {
    { // Keyboard Press
        int key = GetKeyPressed();
        
        while (key != 0) {
            // TODO: TEST IF THIS CAN GET A KEY PRESSED MORE THAN ONCE(do this by pressing on 2 keyboards and not releasing)
            keyboardKeysPressed.emplace_back(key);

            for (auto& func : onKeyboardPressFunctions)
                func(key);
            
            key = GetKeyPressed();
        }
    }

    // Keyboard Release
    for (auto it = keyboardKeysPressed.begin(); it != keyboardKeysPressed.end(); ++it) {
        if (IsKeyUp(*it)) {
            for (auto& func : onKeyboardReleaseFunctions)
                func(*it);
            
            keyboardKeysPressed.erase(it);
            --it;
        }
    }

    // Mouse Press + Mouse Release
    for (int button = 0; button != 6; ++button) {
        if (IsMouseButtonPressed(button))
            for (auto& func : onMousePressFunctions)
                func(button);
        
        if (IsMouseButtonReleased(button))
            for (auto& func : onMouseReleaseFunctions)
                func(button);
    }

    Vector2 mouseWheelMove = GetMouseWheelMoveV();
    if (mouseWheelMove.x != mouseWheelMoveBefore.x || mouseWheelMove.y != mouseWheelMoveBefore.y) {
        for (auto& func : onMouseWheelFunctions)
            func(mouseWheelMove.x, mouseWheelMove.y);
        
        mouseWheelMoveBefore = mouseWheelMove;
    }

    // GetGamepadButtonPressed does not ask for a gamepad id, while IsGamepadButtonReleased asks. This is strange...
    /*{ // Gamepad Press
        int button = GetGamepadButtonPressed();

        while (button != 0) {
            // TODO: TEST IF THIS CAN GET A BUTTON PRESSED MORE THAN ONCE(do this by pressing on 2 gamepads and not releasing)
            gamepadButtonsPressed.emplace_back(button);

            for (auto& func : onGamepadPressFunctions)
                func(button);
            
            button = GetGamepadButtonPressed();
        }
    }

    // Gamepad Release
    for (auto it = gamepadButtonsPressed.begin(); it != gamepadButtonsPressed.end(); ++it) {
        if (IsGamepadButtonReleased(1, *it)) {

        }
    }*/
}