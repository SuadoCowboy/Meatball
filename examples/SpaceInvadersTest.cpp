#include <iostream>
#include <vector>
#include <utility>

#include <raylib.h>

#include <Console.h>
#include <ConsoleUI.h>
#include <Utils/Defaults.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700

int main(int, char**)
{
    // 1st task: create a simple space invaders(2d game) copy using Meatball
    // 2nd task: create a 3d fps shooter?
    // 3rd task: implement multiplayer and ticks per second system

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Console Build In Progress");
    SetWindowState(FLAG_VSYNC_HINT);

    SetExitKey(KEY_NULL); // disable exit key

    Color backgroundColor = GetColor(0x181818FF);

    // Utils/Defaults.h
    auto consoleUI = Meatball::initLocalConsole((Rectangle){WINDOW_WIDTH/4, WINDOW_HEIGHT/4, WINDOW_WIDTH/2, WINDOW_HEIGHT/2}, "data/consoleUI.meatdata");
    
    consoleUI.sendButton.onClick = [&]() {
        consoleUI.print("THIS IS A MESSAGE BEING PRINTED!111\n");
    };

    while (!WindowShouldClose()) {
        ClearBackground(RAYWHITE);
        
        consoleUI.update();

        BeginDrawing();
        consoleUI.draw();
        EndDrawing();
    }

    return 0;
}