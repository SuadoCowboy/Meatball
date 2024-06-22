#include "Game.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

int main(int, char**) {
    auto consoleUI = init(WINDOW_WIDTH, WINDOW_HEIGHT);

    while (!shouldQuit) {
        if (IsWindowResized()) {
            resize(consoleUI);
        }
        
        render(consoleUI);
    }

    cleanup();
}