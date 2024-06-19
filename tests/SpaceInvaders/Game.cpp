#include <raylib.h>

#include <vector>

#include <HayBCMD.h>

#include <Console.h>
#include <ConsoleUI.h>
#include <Utils/Defaults.h>
#include <FontsHandler.h>
#include <Config.h>
#include <Input.h>
#include <MouseCursor.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define MOVEUP 0
#define MOVEDOWN 2
#define NOMOVE 1
#define MOVELEFT 0
#define MOVERIGHT 2

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

struct Vector2uc {
    unsigned char x;
    unsigned char y;
};

struct Player {
    Vector2 position;
    Vector2uc direction;
    Texture2D texture;
    Vector2 velocity;

    inline void update(float dt) {
        position.x += (((short)direction.x)-1)*dt*velocity.x;
        position.y += (((short)direction.y)-1)*dt*velocity.y;
    }

    inline void draw() {
        DrawTexture(texture, position.x, position.y, WHITE);
    }

    Player(const Vector2& position, const Vector2& velocity) : position(position), direction({0,0}), velocity(velocity) {
        texture = LoadTexture("data/images/player.png");
        direction = {NOMOVE, NOMOVE};
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

static Vector2uc* pPlayerDirection;

static unsigned char moves = 0;
/*
1 = up
2 = down
4 = left
8 = right
16 = NON RELATED -> 16 = should quit
*/

void quit(HayBCMD::Command*, const std::vector<std::string>&) {
    moves |= 16;
}

void moveup(HayBCMD::Command*, const std::vector<std::string>&) {
    if (moves & 1) return;
    pPlayerDirection->y--;
    moves |= 1;
}

void movedown(HayBCMD::Command*, const std::vector<std::string>&) {
    if (moves & 2) return;
    pPlayerDirection->y++;
    moves |= 2;
}

void moveleft(HayBCMD::Command*, const std::vector<std::string>&) {
    if (moves & 4) return;
    pPlayerDirection->x--;
    moves |= 4;
}

void moveright(HayBCMD::Command*, const std::vector<std::string>&) {
    if (moves & 8) return;
    pPlayerDirection->x++;
    moves |= 8;
}

void stopmoveup(HayBCMD::Command*, const std::vector<std::string>&) {
    if (!(moves & 1)) return;
    pPlayerDirection->y++;
    moves &= ~1;
}

void stopmovedown(HayBCMD::Command*, const std::vector<std::string>&) {
    if (!(moves & 2)) return;
    pPlayerDirection->y--;
    moves &= ~2;
}

void stopmoveleft(HayBCMD::Command*, const std::vector<std::string>&) {
    if (!(moves & 4)) return;
    pPlayerDirection->x++;
    moves &= ~4;
}

void stopmoveright(HayBCMD::Command*, const std::vector<std::string>&) {
    if (!(moves & 8)) return;
    pPlayerDirection->x--;
    moves &= ~8;
}

void loadCommands(Meatball::ConsoleUIScene& consoleUI) {
    HayBCMD::Command("quit", 0, 0, quit,"- closes the window");

    HayBCMD::Command("+moveup", 0, 0, moveup, "- moves up");
    HayBCMD::Command("+movedown", 0, 0, movedown, "- moves down");
    HayBCMD::Command("+moveleft", 0, 0, moveleft, "- moves left");
    HayBCMD::Command("+moveright", 0, 0, moveright, "- moves right");

    HayBCMD::Command("-moveup", 0, 0, stopmoveup, "- stops moving up");
    HayBCMD::Command("-movedown", 0, 0, stopmovedown, "- stops moving down");
    HayBCMD::Command("-moveleft", 0, 0, stopmoveleft, "- stops moving left");
    HayBCMD::Command("-moveright", 0, 0, stopmoveright, "- stops moving right");

    HayBCMD::Command("reload_fonts", 0, 0, [&](HayBCMD::Command*, const std::vector<std::string>&) {
        Meatball::FontsHandler::clear();
        Meatball::FontsHandler::add(0, GetFontDefault());

        auto consoleData = Config::loadData("data/meatdata/Console.meatdata");

        auto data = Config::ifContainsGet(consoleData, "font");
        std::string path;
        if (data) Meatball::Defaults::loadConsoleFonts(consoleUI,((Meatball::Config::ConfigTypeData<std::string>*)data)->value);

        Meatball::Config::clearData(consoleData);
    }, "- reloads all text fonts.");

    HayBCMD::Command("toggle_local_console", 0, 0, [&](HayBCMD::Command*, const std::vector<std::string>&) {
        consoleUI.visible = not consoleUI.visible;
        if (!consoleUI.visible)
            Meatball::resetCursor(Meatball::MouseCursorPriorityLevel::INPUT_TEXT_BOX);
    }, "- toggles the console ui visibility");

    Meatball::Input::allowedUiCommands.push_back("toggle_local_console");
    Meatball::Input::allowedUiCommands.push_back("quit");
}

int main(int, char**)
{
    // 1st task: create a simple space invaders(2d game) copy using Meatball
    // 2nd task: create a 3d fps shooter?
    // 3rd task: implement multiplayer and ticks per second system

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Console Build In Progress");
    SetExitKey(KEY_NULL); // disable exit key

    {
        SetWindowMinSize(WINDOW_WIDTH*0.5, WINDOW_HEIGHT*0.5);
        int currentMonitorId = GetCurrentMonitor();
        SetWindowMaxSize(GetMonitorWidth(currentMonitorId), GetMonitorHeight(currentMonitorId));
    }

    Meatball::Defaults::init("data/meatdata/Init.meatdata");

    auto consoleUI = initConsole();

    loadCommands(consoleUI);
    Meatball::Input::registerCommands();
    Meatball::Input::mapKeyboardKeys();
    Meatball::Input::mapMouseKeys();

    HayBCMD::execConfigFile("data/cfg/autoexec.cfg", Meatball::Console::variables);
    HayBCMD::execConfigFile("data/cfg/config.cfg", Meatball::Console::variables);

    Texture2D backgroundTexture = LoadTexture("data/images/background.png");
    backgroundTexture.width = GetRenderWidth();
    backgroundTexture.height = GetRenderHeight();

    Player player{{.0f,.0f}, {backgroundTexture.width*0.4f, backgroundTexture.height*0.6f}};
    player.texture.width = backgroundTexture.width*0.05f;
    player.texture.height = backgroundTexture.width*0.05f;

    player.position.x = backgroundTexture.width*0.5f-player.texture.width*0.5f;
    player.position.y = backgroundTexture.height*0.9f-player.texture.height;

    pPlayerDirection = &player.direction;

    std::vector<Enemy> enemies;

    while (!(moves & 16)) {
        if (IsWindowResized()) {
            int newScreenWidth = GetRenderWidth(), newScreenHeight = GetRenderHeight();
            Vector2 ratio = {(float)newScreenWidth/backgroundTexture.width, (float)newScreenHeight/backgroundTexture.height};
            
            consoleUI.onResize(ratio.x, ratio.y);

            backgroundTexture.width = newScreenWidth;
            backgroundTexture.height = newScreenHeight;

            player.position.x *= ratio.x;
            player.position.y *= ratio.y;
            player.texture.width *= ratio.x;
            player.texture.height *= ratio.y;

            player.velocity.x *= ratio.x;
            player.velocity.y *= ratio.y;

            for (auto& enemy : enemies) {
                enemy.position.x *= ratio.x;
                enemy.position.y *= ratio.y;
                enemy.texture.width *= ratio.x;
                enemy.texture.height *= ratio.y;
            }
        }

        ClearBackground(BLACK);
        DrawTexture(backgroundTexture, 0,0, WHITE);

        float dt = GetFrameTime();
        consoleUI.update();

        Meatball::Input::update(consoleUI.visible);

        player.update(dt);
        HayBCMD::handleLoopAliasesRunning(Meatball::Console::variables);

        if (WindowShouldClose()) moves |= 16;

        BeginDrawing();

        player.draw();

        consoleUI.draw();
        EndDrawing();
    }

    Meatball::FontsHandler::clear();
    CloseWindow();
}