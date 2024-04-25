#include "Engine.h"

void Meatball::Engine::init() {
    Console::init();
}

Meatball::Engine::Engine(Scene& currentScene): currentScene(currentScene) {
    float screenWidth = (float)GetScreenWidth();
    float screenHeight = (float)GetScreenHeight();
    
    float consoleUIWidth = screenWidth - screenWidth / 4;
    float consoleUIHeight = screenHeight - screenHeight / 4;

    consoleUI = ConsoleUI(
        screenWidth / 2 - consoleUIWidth / 2,
        screenHeight / 2 - consoleUIHeight / 2,
        consoleUIWidth,
        consoleUIHeight
    ); // centers the console to the middle of the screen
}

Meatball::Engine::~Engine() {}

void Meatball::Engine::handleInput() {
    /*
    // pseudo code for onFocusGain and onFocusLoss FOR CONSOLE UI:
    if (consoleUI.visible && mousebuttonpressed()) {
        if (&mousebuttonpress.object == consoleUI) {
            if (!consoleUI.focused) consoleUI->onFocusGain();
            return;
        } else
            consoleUI->onFocusLoss();
    }
    currentScene.handleInput(); // WHEN ADDING THIS PART OF THE CODE REMEMBER TO REMOVE THE LINE BELOW
    */
    currentScene.handleInput();
}

void Meatball::Engine::update() {
    if (consoleUI.visible) // && consoleUI.focused)
        consoleUI.update();
    currentScene.update();
}

void Meatball::Engine::draw() {
    // in the future allow multiple scenes being drawn
    // imagine a hud being a scene, the main game another scene and the pause menu another scene too
    // that's 3 scenes being drawn
    currentScene.draw();

    if (consoleUI.visible)
        consoleUI.draw();
}