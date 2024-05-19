#include "Engine.h"

void Meatball::Engine::init() {
    Console::init();
}

Meatball::Engine::Engine(Scene* currentScene, unsigned short screenWidth, unsigned short screenHeight)
    : currentScene(currentScene), baseScreenWidth(screenWidth), baseScreenHeight(screenHeight), screenUnitSystem({1, 1}) {
    
    float consoleUIWidth = baseScreenWidth - baseScreenWidth / 4;
    float consoleUIHeight = baseScreenHeight - baseScreenHeight / 4;

    consoleUI = createConsoleUI(
        baseScreenWidth / 2 - consoleUIWidth / 2,
        baseScreenHeight / 2 - consoleUIHeight / 2,
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
    Rectangle mouseRect = {(float)GetMouseX(), (float)GetMouseY(), 1, 1};
    
    if (IsKeyReleased(KEY_GRAVE))
        consoleUI->visible = !consoleUI->visible;

    if (consoleUI->visible) {
        // consoleUI->uINodes[0] = mainPanel
        if (consoleUI->getUINodes().at(0)->checkCollision(mouseRect)) {
            consoleUI->handleInput();
            return;
        }
    }
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
    if (currentScene->visible)
        currentScene->handleInput();
}

void Meatball::Engine::updateNodesSizes() {
    Vector2 oldScreenUnitSystem = screenUnitSystem;

    screenUnitSystem.x = (float)GetScreenWidth() / baseScreenWidth;
    screenUnitSystem.y = (float)GetScreenHeight() / baseScreenHeight;

    if (screenUnitSystem.x == 0) screenUnitSystem.x = 0.1f;
    if (screenUnitSystem.y == 0) screenUnitSystem.y = 0.1f;
    
    for (auto& scene : scenes) for (auto& node : scene->getUINodes()) {
        node->x = node->x / oldScreenUnitSystem.x * screenUnitSystem.x;
        node->y = node->y / oldScreenUnitSystem.y * screenUnitSystem.y;
        node->width = node->width / oldScreenUnitSystem.x * screenUnitSystem.x;
        node->height = node->height / oldScreenUnitSystem.y * screenUnitSystem.y;
        /*if (node->getTypes() & NodeType::NODE2D) {
            Node2D* node2d = (Node2D*)node;
            node2d->velocity.x = node2d->velocity.x/oldScreenUnitSystem.x * screenUnitSystem.x;
            node2d->velocity.y = node2d->velocity.y/oldScreenUnitSystem.y * screenUnitSystem.y;
        }*/

        if (node->getTypes() & NodeType::NODEUI_LABEL) {
            Interface::Label* label = (Interface::Label*)node;

            label->fontSize = label->fontSize / oldScreenUnitSystem.y * screenUnitSystem.y;
        }
    }

    for (auto& scene : scenes) for (auto& node : scene->getUINodes()) {
        Interface::NodeUI* anchoredTo = node->getAnchor();
        if (anchoredTo != nullptr) {
            node->setAnchor(anchoredTo); // updates relative to the new position
        }
    }

    for (auto& node : consoleUI->getUINodes()) {
        node->x = node->x / oldScreenUnitSystem.x * screenUnitSystem.x;
        node->y = node->y / oldScreenUnitSystem.y * screenUnitSystem.y;
        node->width = node->width / oldScreenUnitSystem.x * screenUnitSystem.x;
        node->height = node->height / oldScreenUnitSystem.y * screenUnitSystem.y;

        if (node->getTypes() & NodeType::NODEUI_BUTTON) {
            Console::print("(" + std::to_string(node->x) + ", " + std::to_string(node->y) + ")\n");
        }

        if (node->getTypes() & NodeType::NODEUI_LABEL) {
            Interface::Label* label = (Interface::Label*)node;

            label->fontSize = label->fontSize / oldScreenUnitSystem.y * screenUnitSystem.y;
        }
    }

    for (auto& node : consoleUI->getUINodes()) {
        Interface::NodeUI* anchoredTo = node->getAnchor();
        if (anchoredTo != nullptr) {
            node->setAnchor(anchoredTo); // updates relative to the new position
        }
    }
}

void Meatball::Engine::update() {
    if (IsWindowResized()) updateNodesSizes();

    if (consoleUI->visible)
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