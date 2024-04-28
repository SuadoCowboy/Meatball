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
        
        Engine(Scene* currentScene);
        ~Engine();
        
        void addScene(Scene* scene);
        //void removeScene(Scene* scene); // how would scenes and nodes be removed?

        void handleInput();
        void update();
        void draw(); // draw current scene + consoleUI
    
    private:
        Scene* consoleUI; // console is not inside any scene
        bool consoleUIFocused;

        Scene* currentScene;
        std::vector<Scene*> scenes;
    };
}