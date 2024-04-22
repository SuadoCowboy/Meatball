#include <raylib.h>

#include "Engine.h"

int main(int, char**)
{
    // 1st task: create a simple space invaders(2d game) copy using Meatball
    // 2nd task: create a 3d fps/tps shooter?
    // 3rd task: implement multiplayer and ticks per second system

    InitWindow(800, 600, "My Title UwU");
    SetTargetFPS(60);

    Color backgroundColor = GetColor(0x181818FF);

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