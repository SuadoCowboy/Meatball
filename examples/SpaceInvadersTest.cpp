#include <iostream>
#include <vector>
#include <utility>

#include <raylib.h>

#include <Console.h>
#include <ConsoleUI.h>
#include <Config.h>

#include <HayBCMD.h>

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

    Meatball::Console::init();

    auto consoleUIData = Meatball::loadData("data/consoleUI.meatdata");
    auto consoleUI = Meatball::ConsoleUIScene(WINDOW_WIDTH/4, WINDOW_HEIGHT/4, WINDOW_WIDTH/2, WINDOW_HEIGHT/2); 

    if (consoleUIData.size() < 4) {
        consoleUI.mainPanel.color = BLACK;
        consoleUI.sendButton.setText("WRONG");
        consoleUI.sendButton.color = WHITE;
        consoleUI.closeButton.color = BLACK;

    } else {
        consoleUI.mainPanel.color = consoleUIData["mainPanelColor"].colorV;

        consoleUI.sendButton.setText(consoleUIData["sendButtonText"].stringV);
        consoleUI.sendButton.color = consoleUIData["sendButtonColor"].colorV;

        consoleUI.closeButton.color = consoleUIData["closeButtonColor"].colorV;
    }

    while (!WindowShouldClose()) {
        ClearBackground(RAYWHITE);
        
        consoleUI.update();

        BeginDrawing();
        consoleUI.draw();
        EndDrawing();
    }

    return 0;
}