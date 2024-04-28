#include "Scene.h"

#include "NodeUI.h"
#include "Button.h"
#include "Input.h"

#include <raylib.h>

Meatball::Scene::Scene(int x, int y, int width, int height, bool visible) : Node(x, y, width, height, visible), currentFocusedNode(nullptr) {}

Meatball::Scene::~Scene() {
    for (auto& node : nodes)
        delete node;

    nodes.clear();
}

int Meatball::Scene::getTypes() {
    return Node::getTypes() | NodeType::SCENE;
}

void Meatball::Scene::addNode(Node* node) {
    nodes.push_back(node);
}

void Meatball::Scene::handleInput() {
    // TODO: add mousewheel event

    unsigned char mouseButtons = Input::anyMouseButtonPressed();
    if (mouseButtons != 0) {
        int mouseX = GetMouseX();
        int mouseY = GetMouseY();
        
        bool hitNode = false;
        Rectangle mouseRect{(float)mouseX, (float)mouseY, 1.0f, 1.0f};
        for (auto& node : nodes) {
            if (!node->visible || !node->checkCollision(mouseRect))
                continue;

            if (currentFocusedNode != nullptr && currentFocusedNode->getTypes() & NodeType::NODEUI)
                dynamic_cast<Interface::NodeUI*>(currentFocusedNode)->onFocusLoss();
                
            currentFocusedNode = node;
            dynamic_cast<Interface::NodeUI*>(currentFocusedNode)->onFocusGain();

            hitNode = true;
            break;
            
        }

        // if the hit was in an empty space
        if (currentFocusedNode != nullptr && !hitNode) {
            dynamic_cast<Interface::NodeUI*>(currentFocusedNode)->onFocusLoss();
            currentFocusedNode = nullptr;
        }

        // TODO: find a way to check whether we are in game or using a user interface.
        // FOR NOW we are only using NodeUI so it's okay to stay like that.
        
        // Scene class will check which node object was pressed by a mouse button and call a event
        if (currentFocusedNode != nullptr && currentFocusedNode->getTypes() & NodeType::NODEUI_BUTTON)
            dynamic_cast<Interface::Button*>(currentFocusedNode)->onMouseButtonPressed(
                (Input::InpMouseButton)mouseButtons);
            // maybe button press function should use events like press and release? Idk. See that in the future future
        //else if (currentFocusedNode->getTypes() & NodeType::NODEUI_TEXT_INPUT)
            // handle text input
        
        //else if (currentFocusedNode->getTypes() & NodeType::NODEUI_BUTTON_TOGGLE)
            //dynamic_cast<Interface::ToggleButton*>(currentFocusedNode)->toggle();
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