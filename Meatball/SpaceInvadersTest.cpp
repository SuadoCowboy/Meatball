#include <raylib.h>

#include "Engine.h"

#include <iostream>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700

namespace Input = Meatball::Input;

int main(int, char**)
{
    // 1st task: create a simple space invaders(2d game) copy using Meatball
    // 2nd task: create a 3d fps  shooter?
    // 3rd task: implement multiplayer and ticks per second system

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Console Build In Progress");
    SetWindowState(FLAG_VSYNC_HINT);

    Color backgroundColor = GetColor(0x181818FF);

    Meatball::Engine::init();

    Meatball::Scene* mainScene = new Meatball::Scene{ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    
    Color buttonColor = { 42, 107, 73, 255 };
    Meatball::Interface::Button* myButton = new Meatball::Interface::Button(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 37, 300, 75, buttonColor);

    Font mytypeFont = LoadFont("fonts\\mytype.ttf");
    Meatball::Interface::Label* myLabel = new Meatball::Interface::Label((float)myButton->height, mytypeFont, myButton->x, myButton->y);
    myLabel->backgroundColor = { 0, 0, 0, 127 };
    
    myLabel->setText("Say gex.");
    myLabel->x = myButton->x + myButton->width / 2 - myLabel->width / 2;
    myLabel->setAnchor(myButton);

    SetExitKey(KEY_NULL); // disable exit key

    myButton->connectOnMouseButtonPressed([&](Meatball::Scene& scene, Meatball::Interface::Button& button, Input::InpMouseButton buttons) {
        if (buttons & Input::InpMouseButton::MOUSE1)
            button.x -= 30;
        if (buttons & Input::InpMouseButton::MOUSE2)
            button.x += 30;
    });

    mainScene->addNode(myButton);
    mainScene->addNode(myLabel);

    mainScene->sortUINodesVector();

    Meatball::Engine engine{mainScene};

    while (!WindowShouldClose()) {
        engine.handleInput();
        engine.update();

        BeginDrawing();

        ClearBackground(backgroundColor);
        engine.draw();

        EndDrawing();
    }

    UnloadFont(mytypeFont);

    return 0;
}