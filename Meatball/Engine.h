#pragma once

#include <HayBCMD.h>

#include <vector>

#include "Console.h"
#include "ConsoleUI.h"
#include "NodeUI.h"

namespace Meatball {
    // threaded general purpose class with TPS, 2d/3d rendering(raylib), command prompt(HayBCMD)
    class Engine {
    public:
        static void init(); // initializes everything that requires initialization
        Engine();
        ~Engine();
        
        void handleInput();
        void update();
        void draw();

        // adds your Node to the engine so that it can be drawn and updated
        void addNode(NodeUI);
        //void addNode(Node2D);
        //void addNode(Node3D); // start with 2d nodes and after try 3d
    private:
        size_t currentFocusedUIObject; // index value in uiNodes vector
        
        std::vector<NodeUI> uiNodes;
    };
}