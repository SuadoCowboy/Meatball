#include <raylib.h>

#include <vector>

#include <HayBCMD.h>

#include <ConsoleUI.h>
#include <Utils/Defaults.h>
#include <FontsHandler.h>
#include <Config.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 700

namespace Config = Meatball::Config;

struct Enemy {
    Vector2 position;
    Texture2D texture;

    inline void draw() {
        DrawTexture(texture, position.x, position.y, WHITE);
    }

    Enemy(const Vector2& position) : position(position) {
        texture = LoadTexture("data/images/enemy01.png");
    }
};

struct Player {
    Vector2 position;
    Texture2D texture;

    inline void draw() {
        DrawTexture(texture, position.x, position.y, WHITE);
    }

    Player(const Vector2& position) : position(position) {
        texture = LoadTexture("data/images/player.png");
    }
};

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

    Meatball::Defaults::init("data/meatdata/Init.meatdata");

    auto consoleUI = initConsole();

    Texture2D backgroundTexture = LoadTexture("data/images/background.png");

    HayBCMD::Command("reload_fonts", 0, 0, [&](HayBCMD::Command*, const std::vector<std::string>&) {
        Meatball::FontsHandler::clear();
        Meatball::FontsHandler::add(0, GetFontDefault());

        auto consoleData = Config::loadData("data/meatdata/Console.meatdata");

        auto data = Config::ifContainsGet(consoleData, "font");
        std::string path;
        if (data) Meatball::Defaults::loadConsoleFonts(consoleUI,((Meatball::Config::ConfigTypeData<std::string>*)data)->value);

        Meatball::Config::clearData(consoleData);
    }, "- reloads all text fonts.");

    bool shouldQuit = false;
    HayBCMD::Command("quit", 0, 0, [&](HayBCMD::Command*, const std::vector<std::string>&) {
        shouldQuit = true;
    }, "- closes the window");

    Player player{{.0f,.0f}};
    player.position.x = (backgroundTexture.width-player.texture.width)*0.5f;
    player.position.y = backgroundTexture.height*0.9f-player.texture.height;

    while (!shouldQuit) {
        if (IsWindowResized()) {
            int newScreenWidth = GetRenderWidth(), newScreenHeight = GetRenderHeight();
            consoleUI.onResize((float)newScreenWidth/backgroundTexture.width, (float)newScreenHeight/backgroundTexture.height);

            backgroundTexture.width = newScreenWidth;
            backgroundTexture.height = newScreenHeight;
        }

        if (WindowShouldClose()) shouldQuit = true;

        ClearBackground(BLUE);
        
        consoleUI.update();

        BeginDrawing();
        consoleUI.draw();

        player.draw();

        EndDrawing();
    }

    Meatball::FontsHandler::clear();
    CloseWindow();
}