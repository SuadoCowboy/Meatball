#include <raylib.h>

#include <HayBCMD.h>

#include <ConsoleUI.h>
#include <Utils/Defaults.h>
#include <FontsHandler.h>
#include <Config.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

namespace Config = Meatball::Config;

static Meatball::ConsoleUIScene initConsole() {
    Rectangle consoleUIRect = {0, 0, WINDOW_WIDTH*0.5f, WINDOW_HEIGHT*0.75f};
    consoleUIRect.x = WINDOW_WIDTH*0.5f-consoleUIRect.width*0.5;
    consoleUIRect.y = WINDOW_HEIGHT*0.5f-consoleUIRect.height*0.5;

    // Utils/Defaults.h
    auto consoleUI = Meatball::Defaults::initLocalConsole(
        consoleUIRect,
        "data/meatdata/Console.meatdata");
    
    return consoleUI;
}

int main(int, char**)
{
    // 1st task: create a simple space invaders(2d game) copy using Meatball
    // 2nd task: create a 3d fps shooter?
    // 3rd task: implement multiplayer and ticks per second system

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Console Build In Progress");
    SetWindowState(FLAG_VSYNC_HINT);
    SetExitKey(KEY_NULL); // disable exit key

    Meatball::Defaults::init("data/meatdata/Init.meatdata"); // for now it is sort of useless
    
    auto consoleUI = initConsole();

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