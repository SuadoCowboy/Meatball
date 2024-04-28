#include <raylib.h>

#include "Engine.h"

#include <iostream>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700

int main(int, char**)
{
    // 1st task: create a simple space invaders(2d game) copy using Meatball
    // 2nd task: create a 3d fps/tps shooter?
    // 3rd task: implement multiplayer and ticks per second system

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My Title UwU");
    SetWindowState(FLAG_VSYNC_HINT);

    Color backgroundColor = GetColor(0x181818FF);

    Meatball::Engine::init();

    Meatball::Scene* mainScene = new Meatball::Scene{ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    
    Color buttonColor = { 42, 107, 73, 255 };
    Meatball::Interface::Button* myButton = new Meatball::Interface::Button(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 50, 400, 100, buttonColor);

    unsigned char currentStyle = 0;

    myButton->setOnMouseButtonPressed([&](Meatball::Interface::Button& buttonClass, MouseButton button) {
        if (button != MouseButton::MOUSE_BUTTON_LEFT)
            return;

        currentStyle++;
        if (currentStyle == 3) currentStyle = 0;
        
        buttonClass.setStyle((Meatball::Interface::ButtonStyle)currentStyle);

        if (currentStyle == 1) {
            myButton->width = 4000;
            myButton->x = WINDOW_WIDTH / 2;
            myButton->y = WINDOW_HEIGHT / 2;
        }
        else {
            myButton->width = 400;
            myButton->x = WINDOW_WIDTH / 2 - 200;
            myButton->y = WINDOW_HEIGHT / 2 - 50;
        }
        });

    mainScene->addNode(myButton);
    
    Meatball::Engine engine{mainScene};

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