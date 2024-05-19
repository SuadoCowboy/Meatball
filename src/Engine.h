#pragma once

#include "Console.h"
#include "ConsoleUI.h"

#include "Node.h"

#include "NodeUI.h"
#include "Button.h"
#include "Label.h"

#include "Scene.h"

#include "Input.h"

#include <HayBCMD.h>

#include <vector>

namespace Meatball {
    // !!! IN PROGRESS !!! threaded general purpose class with TPS, 2d/3d rendering(raylib), command prompt(HayBCMD)
    class Engine {
    public:
        static void init(); // initializes everything that requires initialization
        
        Engine(Scene* currentScene, unsigned short screenWidth, unsigned short screenHeight);
        ~Engine();
        
        void addScene(Scene* scene);
        //void removeScene(Scene* scene); // how would scenes and nodes be removed?

        // when window is resized, this is called to change ui/2d nodes correctly
        void updateNodesSizes();

        void handleInput();
        void update();
        void draw(); // draw current scene + consoleUI
    
        const unsigned short baseScreenWidth, baseScreenHeight;
        // Screen Unit System (SUS): It's used by the Engine to make ui/2d nodes be resized correctly
        // when window resizes. The first window size set this to windowSize/windowSize = 1.0f. When
        // the window changes size, it becomes newWindowSize/windowSize = newSUS.
        // then all the nodes have their positions and sizes as well as velocity divided by old SUS
        // and multiplied by new SUS.
        Vector2 screenUnitSystem;

    private:
        Scene* consoleUI; // console is not inside any scene
        bool consoleUIFocused;

        Scene* currentScene;
        std::vector<Scene*> scenes;
    };
}