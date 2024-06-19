#include <raylib.h>

#include <vector>

#include <HayBCMD.h>

#include <Console.h>
#include <ConsoleUI.h>
#include <Utils/Defaults.h>
#include <FontsHandler.h>
#include <Config.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

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
    Vector2 direction;
    Texture2D texture;
    Vector2 velocity;

    inline void update(float dt) {
        if (IsKeyPressed(KEY_W) && direction.y != -1) {
            HayBCMD::Lexer lexer{"+moveup"};
            HayBCMD::Parser parser(&lexer, Meatball::Console::variables);
            parser.parse();
        } else if (IsKeyReleased(KEY_W)) {
            HayBCMD::Lexer lexer{"-moveup"};
            HayBCMD::Parser parser(&lexer, Meatball::Console::variables);
            parser.parse();
        }
        
        if (IsKeyPressed(KEY_S) && direction.y != 1) {
            HayBCMD::Lexer lexer{"+movedown"};
            HayBCMD::Parser parser(&lexer, Meatball::Console::variables);
            parser.parse();
        } else if (IsKeyReleased(KEY_S)) {
            HayBCMD::Lexer lexer{"-movedown"};
            HayBCMD::Parser parser(&lexer, Meatball::Console::variables);
            parser.parse();
        }
        
        if (IsKeyPressed(KEY_A) && direction.x != -1) {
            HayBCMD::Lexer lexer{"+moveleft"};
            HayBCMD::Parser parser(&lexer, Meatball::Console::variables);
            parser.parse();
        } else if (IsKeyReleased(KEY_A)) {
            HayBCMD::Lexer lexer{"-moveleft"};
            HayBCMD::Parser parser(&lexer, Meatball::Console::variables);
            parser.parse();
        }

        if (IsKeyPressed(KEY_D) && direction.x != 1) {
            HayBCMD::Lexer lexer{"+moveright"};
            HayBCMD::Parser parser(&lexer, Meatball::Console::variables);
            parser.parse();
        } else if (IsKeyReleased(KEY_D)) {
            HayBCMD::Lexer lexer{"-moveright"};
            HayBCMD::Parser parser(&lexer, Meatball::Console::variables);
            parser.parse();
        }

        position.x += direction.x*dt*velocity.x;
        position.y += direction.y*dt*velocity.y;
    }

    inline void draw() {
        DrawTexture(texture, position.x, position.y, WHITE);
    }

    Player(const Vector2& position, const Vector2& velocity) : position(position), direction({0,0}), velocity(velocity) {
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
    SetExitKey(KEY_NULL); // disable exit key

    {
        SetWindowMinSize(WINDOW_WIDTH*0.5, WINDOW_HEIGHT*0.5);
        int currentMonitorId = GetCurrentMonitor();
        SetWindowMaxSize(GetMonitorWidth(currentMonitorId), GetMonitorHeight(currentMonitorId));
    }

    Meatball::Defaults::init("data/meatdata/Init.meatdata");

    auto consoleUI = initConsole();

    Texture2D backgroundTexture = LoadTexture("data/images/background.png");
    backgroundTexture.width = GetRenderWidth();
    backgroundTexture.height = GetRenderHeight();

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

    Player player{{.0f,.0f}, {backgroundTexture.width*0.3f, backgroundTexture.height*0.3f}};
    player.texture.width = backgroundTexture.width*0.05f;
    player.texture.height = backgroundTexture.width*0.05f;

    player.position.x = backgroundTexture.width*0.5f-player.texture.width*0.5f;
    player.position.y = backgroundTexture.height*0.9f-player.texture.height;

    std::vector<Enemy> enemies;

    // now this would be too easy for someone to set plr_direction_x 9999
    // so this needs a fix. Maybe private-access cvars or something i don't really know
    // maybe just don't use cvars and create something that allows direct access lol.
    HayBCMD::CVARStorage::cvar("plr_direction_x", &player.direction.x, "");
    HayBCMD::CVARStorage::cvar("plr_direction_y", &player.direction.y, "");

    { // TODO: binds system :P (for now I will just test with pure code)
        HayBCMD::Lexer lexer{"\
        alias +moveup \"plr_direction_y -1\";\
        alias -moveup \"plr_direction_y 0\";\
        alias +movedown \"plr_direction_y 1\";\
        alias -movedown \"plr_direction_y 0\";\
        alias +moveleft \"plr_direction_x -1\";\
        alias -moveleft \"plr_direction_x 0\";\
        alias +moveright \"plr_direction_x 1\";\
        alias -moveright \"plr_direction_x 0\"\
        "};

        HayBCMD::Parser parser(&lexer, Meatball::Console::variables);
        parser.parse();
    }

    while (!shouldQuit) {
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

        if (WindowShouldClose()) shouldQuit = true;

        ClearBackground(BLACK);
        DrawTexture(backgroundTexture, 0,0, WHITE);

        float dt = GetFrameTime();        
        consoleUI.update();

        player.update(dt);

        BeginDrawing();

        player.draw();

        consoleUI.draw();
        EndDrawing();

        HayBCMD::handleLoopAliasesRunning(Meatball::Console::variables);
    }

    Meatball::FontsHandler::clear();
    CloseWindow();
}