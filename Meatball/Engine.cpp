#include "Engine.h"

void Meatball::Engine::init() {
    Console::init();
}

Meatball::Engine::Engine(Scene* currentScene): currentScene(currentScene) {
    float screenWidth = (float)GetScreenWidth();
    float screenHeight = (float)GetScreenHeight();
    
    float consoleUIWidth = screenWidth - screenWidth / 4;
    float consoleUIHeight = screenHeight - screenHeight / 4;

    consoleUI = createConsoleUI(
        screenWidth / 2 - consoleUIWidth / 2,
        screenHeight / 2 - consoleUIHeight / 2,
        consoleUIWidth,
        consoleUIHeight
    ); // centers the console to the middle of the screen
    
    addScene(currentScene);
}

Meatball::Engine::~Engine() {
    for (auto& scene : scenes)
        delete scene;

    scenes.clear();

    if (consoleUI != nullptr) {
        delete consoleUI;
        consoleUI = nullptr;
    }
}

void Meatball::Engine::addScene(Scene* scene) {
    scenes.push_back(scene);
}

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

    /*
    // also handle input outside scene class, create Input class or something
    Input::update();
    // or if Input is not static(altough i don't see a reason why not make it static):
    input.update();
    // Input class should also have something to add the input keys itself but also allow bind command
    // maybe do something like Input::bind(KEY, COMMAND) and Input::bindMouse(KEY, COMMAND) and then the command is
    // parsed by the console ;) (i hope console is fast enough haha...)
    */

    // TODO: code to change focused scenes, when
    currentScene->handleInput();
}

void Meatball::Engine::update() {
    if (consoleUI->visible) // && consoleUI->focused)
        consoleUI->update();

    currentScene->update();
}

void Meatball::Engine::draw() {
    // in the future allow multiple scenes being drawn
    // imagine a hud being a scene, the main game another scene and the pause menu another scene too
    // that's 3 scenes being drawn
    for (auto& scene : scenes)
        if (scene->visible)
            scene->draw();

    if (consoleUI->visible)
        consoleUI->draw();
}