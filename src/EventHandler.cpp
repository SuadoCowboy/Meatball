#include "EventHandler.h"

#include <vector>

std::vector<int> keyboardKeysPressed;
//std::vector<int> Meatball::EventHandler::gamepadButtonsPressed;

Meatball::EventHandler::EventFunc Meatball::EventHandler::onCharPress = [](int){};

std::function<void(int key, bool isRepeat)> Meatball::EventHandler::onKeyboardPress = [](int,bool){};
Meatball::EventHandler::EventFunc Meatball::EventHandler::onKeyboardRelease = Meatball::EventHandler::onCharPress;

Meatball::EventHandler::EventFunc Meatball::EventHandler::onMousePress = Meatball::EventHandler::onCharPress;
Meatball::EventHandler::EventFunc Meatball::EventHandler::onMouseRelease = Meatball::EventHandler::onCharPress;

std::function<void(const Vector2&)> Meatball::EventHandler::onMouseWheel = [](const Vector2&){};

std::function<void(const Vector2&, const Vector2&)> Meatball::EventHandler::onMouseMove = [](const Vector2&,const Vector2&){};

//Meatball::EventHandler::EventFunc> Meatball::EventHandler::onGamepadPress;
//Meatball::EventHandler::EventFunc> Meatball::EventHandler::onGamepadRelease;

Vector2 mouseWheelDirectionBefore = {0.0f,0.0f};

void Meatball::EventHandler::handle() {
    { // ASCII char
        int codepoint = GetCharPressed();

        while (codepoint != 0) {
            onCharPress(codepoint);

            codepoint = GetCharPressed();
        }
    }

    { // Keyboard Press
        int key = GetKeyPressed();
        
        while (key != 0) {
            keyboardKeysPressed.emplace_back(key);

            onKeyboardPress(key, false);
            
            key = GetKeyPressed();
        }
    }

    // Keyboard Release + Keyboard Repeat
    for (auto it = keyboardKeysPressed.begin(); it != keyboardKeysPressed.end(); ++it) {
        if (IsKeyUp(*it)) {
            onKeyboardRelease(*it);
            keyboardKeysPressed.erase(it);
            --it;
        
        } else if (IsKeyPressedRepeat(*it)) {
            onKeyboardPress(*it, true);
        }
    }

    // Mouse Press + Mouse Release
    for (int button = 0; button != 6; ++button) {
        if (IsMouseButtonPressed(button))
            onMousePress(button);
        
        else if (IsMouseButtonReleased(button))
            onMouseRelease(button);
    }

    { // Mouse Wheel
        Vector2 mouseWheelDirection = GetMouseWheelMoveV();
        
        if (mouseWheelDirection.x != mouseWheelDirectionBefore.x || mouseWheelDirection.y != mouseWheelDirectionBefore.y) {
            onMouseWheel(mouseWheelDirection);
            
            mouseWheelDirectionBefore = mouseWheelDirection;
        }
    }

    { // Mouse Move
        Vector2 mouseDelta = GetMouseDelta();

        if (mouseDelta.x != 0 || mouseDelta.y != 0) {
            onMouseMove(mouseDelta, GetMousePosition());
        }
    }

    // GetGamepadButtonPressed does not ask for a gamepad id, while IsGamepadButtonReleased asks. This is strange...
    /*{ // Gamepad Press
        int button = GetGamepadButtonPressed();

        while (button != 0) {
            gamepadButtonsPressed.emplace_back(button);

            for (auto& func : onGamepadPress)
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