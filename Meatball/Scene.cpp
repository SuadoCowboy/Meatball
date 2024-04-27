#include "Scene.h"

Meatball::Scene::Scene(bool visible) : currentFocusedNode(nullptr), visible(visible) {}

Meatball::Scene::~Scene() {
    for (auto& node : nodes)
        delete node;

    nodes.clear();
}

void Meatball::Scene::addNode(Node* node) {
    nodes.push_back(node);
}

void Meatball::Scene::handleInput() {
    if (currentFocusedNode == nullptr || !currentFocusedNode->visible)
        return;

    /*
    //pseudo code for onFocusGain and onFocusLoss:
    if (mousebuttonpressed() && mousebuttonpress.object != currentFocusedNode) {
        currentFocusedNode->onFocusLoss();
        currentFocusedNode = mousebuttonpress.object;
        currentFocusedNode->onFocusGain();
    }
    */
    
    // Scene class will check which node object was pressed by a mouse button and call a event
    switch (currentFocusedNode->type) {
    case TEXT_INPUT_BOX:    
        // place a function call or slice of code here to handle text input
        break;
    case TOGGLE_BUTTON: // a toggle button
        if (UI::ToggleButton* toggleButton = dynamic_cast<UI::ToggleButton*>(currentFocusedNode)) {
            toggleButton->toggle(); // Now it's possible to use variables of button class
        }
        break;
    case BUTTON: // just a simple press button
        if (UI::Button* button = dynamic_cast<UI::Button*>(currentFocusedNode)) {
            button->press(); // Now it's possible to use variables of button class
            // maybe button press function should use events like press and release? Idk. See that in the future future
        }
        break;
    case default: // any other thing that does not require input handling
        break;
    }
}

void Meatball::Scene::update() {
    for (auto& node : nodes)
        node->update(); // should it be like that or just the focused one should be updated?? or maybe the visible ones???
}

void Meatball::Scene::draw() {
    for (auto& node : nodes)
        if (node->visible)
            node->draw();
}