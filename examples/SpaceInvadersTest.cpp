#include <vector>

#include <raylib.h>

#include <HayBCMD.h>
#include <ConsoleUI.h>
#include <Utils/Defaults.h>
#include <Config.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700

namespace Config = Meatball::Config;

static void emptyCommand(HayBCMD::Command* pCommand, const std::vector<std::string>& args) {}

int main(int, char**)
{
    // 1st task: create a simple space invaders(2d game) copy using Meatball
    // 2nd task: create a 3d fps shooter?
    // 3rd task: implement multiplayer and ticks per second system

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Console Build In Progress");
    SetWindowState(FLAG_VSYNC_HINT);
    SetExitKey(KEY_NULL); // disable exit key
    
    // Utils/Defaults.h
    auto consoleUI = Meatball::initLocalConsole(
        {WINDOW_WIDTH/4, WINDOW_HEIGHT/4, WINDOW_WIDTH/2, WINDOW_HEIGHT/2},
        "data/Console.meatdata");

    Color backgroundColor = RAYWHITE;
    {
        auto mainSceneData = Config::loadData("data/MainScene.meatdata");
        auto backgroundColorData = Config::ifContainsGet(mainSceneData, "backgroundColor");
        if (backgroundColorData != nullptr)
            backgroundColor = backgroundColorData->colorV;
    }

    for (unsigned char i = 0; i < 100; i++)
        HayBCMD::Command(HayBCMD::formatString("empty{}", (int)i), 0, 0, emptyCommand, "that's just a empty function frfr");

    while (!WindowShouldClose()) {
        ClearBackground(backgroundColor);
        
        consoleUI.update();

        BeginDrawing();
        consoleUI.draw();

        EndDrawing();
    }

    return 0;
}