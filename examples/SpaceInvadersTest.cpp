#include <vector>

#include <raylib.h>

#include <HayBCMD.h>
#include <ConsoleUI.h>
#include <Utils/Defaults.h>
#include <FontsHandler.h>
#include <Config/Config.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700

namespace Config = Meatball::Config;

int main(int, char**)
{
    // 1st task: create a simple space invaders(2d game) copy using Meatball
    // 2nd task: create a 3d fps shooter?
    // 3rd task: implement multiplayer and ticks per second system

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Console Build In Progress");
    SetWindowState(FLAG_VSYNC_HINT);
    SetExitKey(KEY_NULL); // disable exit key
    
    Meatball::init(); // for now it is sort of useless

    // Utils/Defaults.h
    auto consoleUI = Meatball::initLocalConsole(
        {WINDOW_WIDTH/4, WINDOW_HEIGHT/4, WINDOW_WIDTH/2, WINDOW_HEIGHT/2},
        "data/meatdata/Console.meatdata");

    Color backgroundColor = RAYWHITE;
    {
        auto mainSceneData = Config::loadData("data/meatdata/MainScene.meatdata");
        auto backgroundColorData = Config::ifContainsGet(mainSceneData, "backgroundColor");
        if (backgroundColorData != nullptr)
            backgroundColor = backgroundColorData->colorV;
    }

    while (!WindowShouldClose()) {
        ClearBackground(backgroundColor);
        
        consoleUI.update();

        BeginDrawing();
        consoleUI.draw();

        EndDrawing();
    }

    Meatball::FontsHandler::clear();
}