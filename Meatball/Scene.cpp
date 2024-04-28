#include "Scene.h"

#include "Button.h"
#include "Input.h"

#include <algorithm>

#include <raylib.h>

Meatball::Scene::Scene(int x, int y, int width, int height, bool visible) : NodeUI(x, y, width, height, visible), currentFocusedUINode(nullptr) {}

Meatball::Scene::~Scene() {
    for (auto& node : uiNodes)
        delete node;

    uiNodes.clear();

    /*
    for (auto& node : 2dNodes)
        delete node;
    
    2dNodes.clear();

    for (auto& node : 3dNodes)
        delete node;

    3dNodes.clear();
    */
}

int Meatball::Scene::getTypes() {
    return Node::getTypes() | NodeType::NODEUI_SCENE;
}

void Meatball::Scene::addNode(Interface::NodeUI* node) {
    uiNodes.push_back(node);
}

bool sortByZOrder(Meatball::Node* a, Meatball::Node* b) {
    return a->z > b->z; // Sort in descending order of zOrder/layers
}

void Meatball::Scene::sortUINodesVector() {
    std::sort(uiNodes.begin(), uiNodes.end(), sortByZOrder);
}

void Meatball::Scene::handleInput() {
    // TODO: add mousewheel event

    unsigned char mouseButtons = Input::anyMouseButtonPressed();
    if (mouseButtons == 0)
        return;
        
    bool hitNode = false;
    Rectangle mouseRect{(float)GetMouseX()-x, (float)GetMouseY()-y, 1.0f, 1.0f};

    for (auto it = uiNodes.rbegin(); it != uiNodes.rend(); ++it) {
        auto node = *it;
        if (node->inputPassThrough || !node->visible || !node->checkCollision(mouseRect))
            continue;
        
        if (currentFocusedUINode != nullptr)
            currentFocusedUINode->onFocusLoss(*this);
                
        currentFocusedUINode = node;
        currentFocusedUINode->onFocusGain(*this);

        hitNode = true;
        break;
            
    }

    if (currentFocusedUINode != nullptr) {
        // if the hit was in an empty space
        if (!hitNode) {
            currentFocusedUINode->onFocusLoss(*this);
            currentFocusedUINode = nullptr;
        }
        else if (currentFocusedUINode->getTypes() & NodeType::NODEUI_BUTTON)
            ((Interface::Button*)currentFocusedUINode)->onMouseButtonPressed(
                *this,
                (Input::InpMouseButton)mouseButtons
            );
        // maybe button press function should use events like press and release? Idk. See that in the future future
        //else if (currentFocusedNode->getTypes() & NodeType::NODEUI_TEXT_INPUT)
            // handle text input
        
        //else if (currentFocusedNode->getTypes() & NodeType::NODEUI_BUTTON_TOGGLE)
            //dynamic_cast<Interface::ToggleButton*>(currentFocusedNode)->toggle();
    }

    // TODO: find a way to check whether we are in game or using a user interface.
    // FOR NOW we are only using NodeUI so it's okay to stay like that.
}

void Meatball::Scene::update() {
    for (auto& node : uiNodes)
        node->update(); // should it be like that or just the focused one should be updated?? or maybe the visible ones???
}

void Meatball::Scene::draw() {
    for (auto& node : uiNodes)
        if (node->visible) {
            int originalX = node->x, originalY = node->y;

            node->x += x;
            node->y += y;
            
            node->draw();

            node->x = originalX;
            node->y = originalY;
        }
}