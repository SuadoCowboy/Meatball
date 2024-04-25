#include <raylib.h>

#include "Engine.h"

#include <iostream>

int main(int, char**)
{
    // 1st task: create a simple space invaders(2d game) copy using Meatball
    // 2nd task: create a 3d fps/tps shooter?
    // 3rd task: implement multiplayer and ticks per second system

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1000, 700, "My Title UwU");
    SetWindowState(FLAG_VSYNC_HINT);

    Color backgroundColor = GetColor(0x181818FF);

    Meatball::Engine::init();
    Meatball::Engine engine;

    while (!WindowShouldClose()) {
        engine.handleInput();
        engine.update();

        BeginDrawing();

        ClearBackground(backgroundColor);
        engine.draw();

        EndDrawing();
    }

    return 0;
}