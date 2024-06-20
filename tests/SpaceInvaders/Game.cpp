#include <raylib.h>

#include <vector>
#include <unordered_map>

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

enum EntityType {
    PLAYER = 0,
    ENEMY_WEAK,
    ENEMY_MEDIUM,
    ENEMY_STRONG
};

struct EntityData {
    unsigned char damage;
    unsigned char defaultHealth;
    Color color;
};

static float bulletSpeed;
static Vector2 bulletSize;

static std::unordered_map<EntityType, EntityData> entityData;

struct Bullet {
    EntityType ownerType;
    Vector2 position;

    Bullet(const EntityType& ownerType, const Vector2& position)
        : ownerType(ownerType), position(position) {}
};

std::vector<Bullet> bullets;

class Enemy {
public:
    Enemy(const Vector2& position, const EntityType& type) : position(position), type(type) {
        texture = LoadTexture("data/images/enemy01.png");
        health = entityData[type].defaultHealth;
    }

    inline void draw() {
        DrawTexture(texture, position.x, position.y, WHITE);
    }

    inline void shoot() {
        bullets.push_back({type, {position.x+texture.width*0.5f-bulletSize.x*0.5f, position.y+texture.height*0.5f-bulletSize.y*0.25f}});
    }

    Vector2 position;
    Texture2D texture;
    EntityType type;
    unsigned char damage;
    short health;
};

struct Vector2uc {
    unsigned char x;
    unsigned char y;
};

class Player {
public:
    Player() {}

    Player(const Vector2& position, const Vector2& speed) : position(position), direction({0,0}), speed(speed) {
        texture = LoadTexture("data/images/player.png");
        direction = {NOMOVE, NOMOVE};
        health = 100;
    }

    inline void update(float dt) {
        position.x += (((short)direction.x)-1)*dt*speed.x;
        position.y += (((short)direction.y)-1)*dt*speed.y;
    }

    inline void draw() {
        DrawTexture(texture, position.x, position.y, WHITE);
    }

    Vector2 position;
    Vector2uc direction;
    Texture2D texture;
    Vector2 speed;
    short health;
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

static Player player;

static unsigned char conditions = 0;
/*
1 = up
2 = down
4 = left
8 = right
16 = NON RELATED -> 16 = should quit
*/

void quit(HayBCMD::Command*, const std::vector<std::string>&) {
    conditions |= 16;
}

void moveup(HayBCMD::Command*, const std::vector<std::string>&) {
    if (conditions & 1) return;
    player.direction.y--;
    conditions |= 1;
}

void movedown(HayBCMD::Command*, const std::vector<std::string>&) {
    if (conditions & 2) return;
    player.direction.y++;
    conditions |= 2;
}

void moveleft(HayBCMD::Command*, const std::vector<std::string>&) {
    if (conditions & 4) return;
    player.direction.x--;
    conditions |= 4;
}

void moveright(HayBCMD::Command*, const std::vector<std::string>&) {
    if (conditions & 8) return;
    player.direction.x++;
    conditions |= 8;
}

void stopmoveup(HayBCMD::Command*, const std::vector<std::string>&) {
    if (!(conditions & 1)) return;
    player.direction.y++;
    conditions &= ~1;
}

void stopmovedown(HayBCMD::Command*, const std::vector<std::string>&) {
    if (!(conditions & 2)) return;
    player.direction.y--;
    conditions &= ~2;
}

void stopmoveleft(HayBCMD::Command*, const std::vector<std::string>&) {
    if (!(conditions & 4)) return;
    player.direction.x++;
    conditions &= ~4;
}

void stopmoveright(HayBCMD::Command*, const std::vector<std::string>&) {
    if (!(conditions & 8)) return;
    player.direction.x--;
    conditions &= ~8;
}

void fire(HayBCMD::Command*, const std::vector<std::string>&) {
    bullets.push_back({PLAYER, {player.position.x+player.texture.width*0.5f-bulletSize.x*0.5f, player.position.y+bulletSize.y*0.25f}});
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

    HayBCMD::Command("+fire", 0, 0, fire, "- shoots a bullet");
    Meatball::Console::variables["-fire"] = " ";

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

    {
        entityData[PLAYER] = {30, 90, {255,255,0,255}};
        entityData[ENEMY_WEAK] = {10, 30, {0,0,255,255}};
        entityData[ENEMY_MEDIUM] = {25, 60, {255,0,0,255}};
        entityData[ENEMY_STRONG] = {40, 90, {0,255,0,255}};

        auto dataMap = Meatball::Config::loadData("data/meatdata/Game.meatdata");
        auto data = Meatball::Config::ifContainsGet(dataMap, "playerBulletColor");
        if (data) entityData[PLAYER].color = Meatball::Config::getConfig<Color>(data)->value;

        data = Meatball::Config::ifContainsGet(dataMap, "enemy0BulletColor");
        if (data) entityData[ENEMY_WEAK].color = Meatball::Config::getConfig<Color>(data)->value;

        data = Meatball::Config::ifContainsGet(dataMap, "enemy1BulletColor");
        if (data) entityData[ENEMY_MEDIUM].color = Meatball::Config::getConfig<Color>(data)->value;

        data = Meatball::Config::ifContainsGet(dataMap, "enemy2BulletColor");
        if (data) entityData[ENEMY_STRONG].color = Meatball::Config::getConfig<Color>(data)->value;

        Meatball::Config::clearData(dataMap);
    }

    loadCommands(consoleUI);
    Meatball::Input::registerCommands();
    Meatball::Input::mapKeyboardKeys();
    Meatball::Input::mapMouseKeys();

    Texture2D backgroundTexture = LoadTexture("data/images/background.png");
    backgroundTexture.width = GetRenderWidth();
    backgroundTexture.height = GetRenderHeight();

    player = {{.0f,.0f}, {backgroundTexture.width*0.4f, backgroundTexture.height*0.6f}};
    player.texture.width = backgroundTexture.width*0.05f;
    player.texture.height = backgroundTexture.width*0.05f;

    player.position.x = backgroundTexture.width*0.5f-player.texture.width*0.5f;
    player.position.y = backgroundTexture.height*0.9f-player.texture.height;

    bulletSpeed = backgroundTexture.height*0.6f;

    HayBCMD::CVARStorage::setCvar("bullet_speed",
        [](const std::string& value){bulletSpeed = std::stof(value);},
        [](){return std::to_string(bulletSpeed);}, "");
    HayBCMD::CVARStorage::setCvar("player_health",
        [](const std::string& value){player.health = (unsigned char)std::stoi(value);},
        [](){return std::to_string(player.health);}, "");

    HayBCMD::execConfigFile("data/cfg/autoexec.cfg", Meatball::Console::variables);
    HayBCMD::execConfigFile("data/cfg/config.cfg", Meatball::Console::variables);

    std::vector<Enemy> enemies = {Enemy({50.0f, 50.0f}, ENEMY_WEAK)};

    bulletSize.x = backgroundTexture.width*0.01f;
    bulletSize.y = backgroundTexture.height*0.02f;

    while (!(conditions & 16)) {
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

            bulletSpeed *= ratio.y;
            bulletSize.x *= ratio.x;
            bulletSize.y *= ratio.y;

            player.speed.x *= ratio.x;
            player.speed.y *= ratio.y;

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

        HayBCMD::handleLoopAliasesRunning(Meatball::Console::variables);
        if (WindowShouldClose()) conditions |= 16;

        BeginDrawing();

        player.update(dt);
        player.draw();

        Rectangle playerRect = {player.position.x, player.position.y, (float)player.texture.width, (float)player.texture.height};
        for (size_t i = 0; i < enemies.size(); ++i) {
            if (CheckCollisionRecs(playerRect, {enemies[i].position.x, enemies[i].position.y, (float)enemies[i].texture.width, (float)enemies[i].texture.height})) {
                player.health -= enemies[i].health;
                enemies.erase(enemies.begin()+i);
                --i;
                continue;
            }
            
            enemies[i].draw();
        }

        for (size_t i = 0; i < bullets.size(); ++i) {
            Rectangle rect = {bullets[i].position.x, bullets[i].position.y, bulletSize.x, bulletSize.y};
            if (bullets[i].position.y > backgroundTexture.height) {
                bullets.erase(bullets.begin()+i);
                --i;
                continue;
            }

            if (bullets[i].ownerType == PLAYER) {
                bool hitEnemy = false;
                for (size_t ii = 0; ii < enemies.size(); ++ii) {
                    if (!CheckCollisionRecs(rect, {enemies[ii].position.x, enemies[ii].position.y, (float)enemies[ii].texture.width, (float)enemies[ii].texture.height}))
                        continue;
                    
                    enemies[ii].health -= entityData[bullets[i].ownerType].damage;
                    hitEnemy = true;
                    
                    if (enemies[i].health < 0) {
                        enemies.erase(enemies.begin()+ii);
                        --ii;
                        continue;
                    }
                    break;
                }

                if (hitEnemy) {
                    bullets.erase(bullets.begin()+i);
                    --i;
                    continue;
                }

                bullets[i].position.y -= bulletSpeed*dt;
            }
            else {
                if (CheckCollisionRecs(rect, {player.position.x, player.position.y, (float)player.texture.width, (float)player.texture.height})) {
                    player.health -= entityData[bullets[i].ownerType].damage;
                    bullets.erase(bullets.begin()+i);
                    --i;
                    continue;
                }

                bullets[i].position.y += bulletSpeed*dt;
            }

            DrawRectangle(rect.x, rect.y, rect.width, rect.height, entityData[bullets[i].ownerType].color);
        }

        consoleUI.draw();
        EndDrawing();
    }

    Meatball::FontsHandler::clear();
    CloseWindow();
}