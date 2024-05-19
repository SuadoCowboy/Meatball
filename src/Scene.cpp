#include "Scene.h"

#include "Button.h"
#include "Input.h"
#include "Label.h"

#include <algorithm>

Meatball::Scene::Scene(bool visible) : visible(visible), currentFocusedUINode(nullptr) {}

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
    // TODO: add mousewheel 
    unsigned char mouseButtons = Input::anyMouseButtonPressed();
    if (mouseButtons == 0)
        return;
        
    bool hitNode = false;
    Rectangle mouseRect{(float)GetMouseX(), (float)GetMouseY(), 1.0f, 1.0f};

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
            //float originalX = node->x, originalY = node->y;

            /*node->x *= screenUnitSystem.x;
            node->y *= screenUnitSystem.y;
            node->width *= screenUnitSystem.x;
            node->height *= screenUnitSystem.y;

            float originalFontSize = 0.0f;
            if (node->getTypes() & NodeType::NODEUI_LABEL) {
                auto label = (Interface::Label*)node;
                originalFontSize = label->fontSize;
                label->fontSize *= screenUnitSystem.y;
            }*/
            
            node->draw();

            /*node->x = originalX;
            node->y = originalY;
            node->width = originalWidth;
            node->height = originalHeight;

            if (node->getTypes() & NodeType::NODEUI_LABEL) {
                ((Interface::Label*)node)->fontSize = originalFontSize;
            }*/
        }
}

const std::vector<Meatball::Interface::NodeUI*>& Meatball::Scene::getUINodes() {
    return uiNodes;
}

Meatball::Interface::NodeUI* Meatball::Scene::getCurrentFocusedUINode() {
    return currentFocusedUINode;
}

/*
const std::vector<Meatball::Interface::Node2D*>& Meatball::Scene::get2DNodes() {
    return 2dNodes;
}

const std::vector<Meatball::Interface::Node3D*>& Meatball::Scene::get3DNodes() {
    return 3dNodes;
}
*/