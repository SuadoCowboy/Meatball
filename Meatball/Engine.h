#pragma once

#include "Console.h"
#include "ConsoleUI.h"
#include "Node.h"
#include "Scene.h"

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
        ConsoleUI consoleUI; // console is not inside any scene
        
        Scene* currentScene;
        std::vector<Scene*> scenes;
    };
}