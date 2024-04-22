#include "Engine.h"

void Meatball::Engine::init() {
    Console::init();
}

void Meatball::Engine::addNode(NodeUI node) {
    uiNodes.push_back(node);
}

Meatball::Engine::Engine(): currentFocusedUIObject(0) {
    ConsoleUI consoleUI{};
    addNode(consoleUI); // console is index 0
}

Meatball::Engine::~Engine() {
    //for (auto& node : uiNodes)
        //node.unload()
}

void Meatball::Engine::handleInput() {
    for (auto& node : uiNodes) {
        if (node.visible)
            node.handleInput();
    }
}

void Meatball::Engine::update() {
    for (auto& node : uiNodes)
        node.update();
}

void Meatball::Engine::draw() {
    for (auto& node : uiNodes)
        if (node.visible)
            node.draw();
}