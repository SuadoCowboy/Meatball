#include "Scene.h"

Meatball::Scene::Scene(): currentFocusedUINodeIndex(0) {}

Meatball::Scene::~Scene() {
    for (auto& node : uiNodes)
        delete node;

    uiNodes.clear();
}

void Meatball::Scene::addNode(NodeUI* node) {
    uiNodes.push_back(node);
}

void Meatball::Scene::handleInput() {
    /*
    //pseudo code for onFocusGain and onFocusLoss:
    if (mousebuttonpressed() && mousebuttonpress.object != uiNodes[currentFocusedUINodeIndex]) {
        uiNodes[currentFocusedUINodeIndex]->onFocusLoss();
        currentFocusedUIObjectIndex = mousebuttonpress.index; // something like this
        uiNodes[currentFocusedUINodeIndex]->onFocusGain();
    }
    */

    if (uiNodes[currentFocusedUINodeIndex]->visible)
        uiNodes[currentFocusedUINodeIndex]->handleInput();
}

void Meatball::Scene::update() {
    for (auto& node : uiNodes)
        node->update(); // should it be like that or just the focused one should be updated?? or maybe the visible ones???
}

void Meatball::Scene::draw() {
    for (auto& node : uiNodes)
        if (node->visible)
            node->draw();
}