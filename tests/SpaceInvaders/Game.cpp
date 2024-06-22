#include <vector>
#include <unordered_map>
#include <cmath>
#include <chrono>

#include <raylib.h>

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
    Texture2D texture;
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
        health = entityData[type].defaultHealth;
    }

    inline void draw() {
        DrawTexture(entityData[type].texture, position.x, position.y, WHITE);
    }

    inline void shoot() {
        bullets.push_back({type, {position.x+entityData[type].texture.width*0.5f-bulletSize.x*0.5f, position.y+entityData[type].texture.height*0.5f-bulletSize.y*0.25f}});
    }

    Vector2 position;
    EntityType type;
    unsigned char damage;
    short health;
};

static std::vector<Enemy> enemies;

struct Vector2uc {
    unsigned char x;
    unsigned char y;
};

class Player {
public:
    Player() {}

    Player(const Vector2& position, const Vector2& speed) : position(position), direction({0,0}), speed(speed) {
        direction = {NOMOVE, NOMOVE};
        health = 100;
    }

    inline void update(float dt, int renderWidth, int renderHeight) {
        position.x += (((short)direction.x)-1)*dt*speed.x*renderWidth;
        position.y += (((short)direction.y)-1)*dt*speed.y*renderHeight;
    }

    inline void draw() {
        DrawTexture(entityData[PLAYER].texture, position.x, position.y, WHITE);
    }

    Vector2 position;
    Vector2uc direction;
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

static bool shouldQuit = false;

void quit(void*, const std::vector<std::string>&) {
    shouldQuit = true;
}

// TODO: cooldown should be handled with TPS(Ticks Per Second) System
//short shootCooldown = 500; // in ms

void loadCommands(Meatball::ConsoleUIScene& consoleUI) {
    HayBCMD::Command("quit", 0, 0, quit,"- closes the window");

    HayBCMD::Command("+moveup", 0, 0, [](void*, const std::vector<std::string>&){player.direction.y--;},
        "- moves up");
    HayBCMD::Command("+movedown", 0, 0, [](void*, const std::vector<std::string>&){player.direction.y++;},
        "- moves down");
    HayBCMD::Command("+moveleft", 0, 0, [](void*, const std::vector<std::string>&){player.direction.x--;},
        "- moves left");
    HayBCMD::Command("+moveright", 0, 0, [](void*, const std::vector<std::string>&){player.direction.x++;},
        "- moves right");

    HayBCMD::Command("-moveup", 0, 0, [](void*, const std::vector<std::string>&){player.direction.y++;},
        "- stops moving up");
    HayBCMD::Command("-movedown", 0, 0, [](void*, const std::vector<std::string>&){player.direction.y--;},
        "- stops moving down");
    HayBCMD::Command("-moveleft", 0, 0, [](void*, const std::vector<std::string>&){player.direction.x++;},
        "- stops moving left");
    HayBCMD::Command("-moveright", 0, 0, [](void*, const std::vector<std::string>&){player.direction.x--;},
        "- stops moving right");

    HayBCMD::Command("+fire", 0, 0, [](void*, const std::vector<std::string>&) {
        bullets.push_back({PLAYER, {player.position.x+entityData[PLAYER].texture.width*0.5f-bulletSize.x*0.5f, player.position.y+bulletSize.y*0.25f}});
    }, "- shoots a bullet");
    
    Meatball::Console::variables["-fire"] = " ";

    HayBCMD::Command("reload_fonts", 0, 0, [&](void*, const std::vector<std::string>&) {
        Meatball::FontsHandler::clear();
        Meatball::FontsHandler::add(0, GetFontDefault());

        auto consoleData = Config::loadData("data/meatdata/Console.meatdata");

        auto data = Config::ifContainsGet(consoleData, "font");
        std::string path;
        if (data) Meatball::Defaults::loadConsoleFonts(consoleUI,((Meatball::Config::ConfigTypeData<std::string>*)data)->value);

        Meatball::Config::clearData(consoleData);
    }, "- reloads all text fonts.");

    HayBCMD::Command("toggle_local_console", 0, 0, [&](void*, const std::vector<std::string>&) {
        consoleUI.visible = not consoleUI.visible;
        if (!consoleUI.visible)
            Meatball::resetCursor(Meatball::MouseCursorPriorityLevel::INPUT_TEXT_BOX);
    }, "- toggles the console ui visibility");

    Meatball::Input::allowedUiCommands.push_back("toggle_local_console");
    Meatball::Input::allowedUiCommands.push_back("quit");
}

bool handleBullet(size_t& bulletIdx, const float& dt, Texture2D& backgroundTexture) {
    Rectangle rect = {bullets[bulletIdx].position.x, bullets[bulletIdx].position.y, bulletSize.x, bulletSize.y};

    float movement = (bulletSpeed * backgroundTexture.height * dt);
    unsigned int substeps = std::ceil(movement / bulletSize.y);
    float substepMovement = movement/substeps;
    for (unsigned int i = 0; i < substeps; ++i) {
       rect.y += substepMovement * (bullets[bulletIdx].ownerType == PLAYER? -1 : 1);

        if (bullets[bulletIdx].position.y > backgroundTexture.height || bullets[bulletIdx].position.y+bulletSize.y < 0) {
            bullets.erase(bullets.begin()+bulletIdx);
            return false;
        }

        if (bullets[bulletIdx].ownerType == PLAYER) {
            bool hitEnemy = false;

            for (size_t enemyIdx = 0; enemyIdx < enemies.size(); ++enemyIdx) {
                if (!CheckCollisionRecs(rect, {enemies[enemyIdx].position.x, enemies[enemyIdx].position.y, (float)entityData[enemies[enemyIdx].type].texture.width, (float)entityData[enemies[enemyIdx].type].texture.height}))
                    continue;
                
                enemies[enemyIdx].health -= entityData[bullets[bulletIdx].ownerType].damage;
                hitEnemy = true;
                
                if (enemies[enemyIdx].health <= 0) {
                    enemies.erase(enemies.begin()+enemyIdx);
                    --enemyIdx;
                }
                break;
            }

            if (hitEnemy) {
                bullets.erase(bullets.begin()+bulletIdx);
                return false;
            }
        }
        else {
            if (CheckCollisionRecs(rect, {player.position.x, player.position.y, (float)entityData[PLAYER].texture.width, (float)entityData[PLAYER].texture.height})) {
                player.health -= entityData[bullets[bulletIdx].ownerType].damage;
                bullets.erase(bullets.begin()+bulletIdx);
                return false;
            }
        }

    }
    
    bullets[bulletIdx].position.y = rect.y;
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, entityData[bullets[bulletIdx].ownerType].color);
    return true;
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
    consoleUI.visible = false;

    {
        entityData[PLAYER] = {10, 90, {255,255,0,255}, LoadTexture("data/images/player.png")};
        entityData[ENEMY_WEAK] = {10, 30, {0,0,255,255}, LoadTexture("data/images/enemy0.png")};
        entityData[ENEMY_MEDIUM] = {25, 60, {255,0,0,255}, LoadTexture("data/images/enemy1.png")};
        entityData[ENEMY_STRONG] = {40, 90, {0,255,0,255}, LoadTexture("data/images/enemy2.png")};

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

    player = {{.0f,.0f}, {0.4f, 0.6f}};
    entityData[PLAYER].texture.width = backgroundTexture.width*0.05f;
    entityData[PLAYER].texture.height = backgroundTexture.width*0.05f;

    player.position.x = backgroundTexture.width*0.5f-entityData[PLAYER].texture.width*0.5f;
    player.position.y = backgroundTexture.height*0.9f-entityData[PLAYER].texture.height;

    bulletSpeed = 0.6f;

    HayBCMD::CVARStorage::setCvar("bullet_speed",
        [](const std::string& value){
            bulletSpeed = std::stof(value);
            if (bulletSpeed < 0.0f)
                bulletSpeed = 0.0f;
        },
        [](){return std::to_string(bulletSpeed);}, "");
    HayBCMD::CVARStorage::setCvar("player_health",
        [](const std::string& value){player.health = (unsigned char)std::stoi(value);},
        [](){return std::to_string(player.health);}, "");

    HayBCMD::CVARStorage::setCvar("player_speed_x",
        [](const std::string& value) {player.speed.x = std::stof(value);},
        [](){
            return std::to_string(player.speed.x);
        }, "");
    
    HayBCMD::CVARStorage::setCvar("player_speed_y",
        [](const std::string& value) {player.speed.y = std::stof(value);},
        [](){
            return std::to_string(player.speed.y);
        }, "");

    HayBCMD::execConfigFile("data/cfg/autoexec.cfg", Meatball::Console::variables);
    HayBCMD::execConfigFile("data/cfg/config.cfg", Meatball::Console::variables);

    enemies = {Enemy({WINDOW_WIDTH*0.5f, WINDOW_HEIGHT*0.5f}, ENEMY_WEAK)};

    bulletSize.x = backgroundTexture.width*0.01f;
    bulletSize.y = backgroundTexture.height*0.04f;

    while (!shouldQuit) {
        if (IsWindowResized()) {
            int newScreenWidth = GetRenderWidth(), newScreenHeight = GetRenderHeight();
            Vector2 ratio = {(float)newScreenWidth/backgroundTexture.width, (float)newScreenHeight/backgroundTexture.height};
            
            consoleUI.onResize(ratio.x, ratio.y);

            backgroundTexture.width = newScreenWidth;
            backgroundTexture.height = newScreenHeight;

            player.position.x *= ratio.x;
            player.position.y *= ratio.y;
            entityData[PLAYER].texture.width *= ratio.x;
            entityData[PLAYER].texture.height *= ratio.y;

            bulletSize.x *= ratio.x;
            bulletSize.y *= ratio.y;

            for (auto& bullet : bullets) {
                bullet.position.x *= ratio.x;
                bullet.position.y *= ratio.y;
            }

            for (auto& enemy : enemies) {
                enemy.position.x *= ratio.x;
                enemy.position.y *= ratio.y;
                entityData[enemy.type].texture.width *= ratio.x;
                entityData[enemy.type].texture.height *= ratio.y;
            }
        }

        ClearBackground(BLACK);
        DrawTexture(backgroundTexture, 0,0, WHITE);

        float dt = GetFrameTime();

        consoleUI.update();

        Meatball::Input::update(consoleUI.visible);

        HayBCMD::handleLoopAliasesRunning(Meatball::Console::variables);
        if (WindowShouldClose()) shouldQuit = true;

        BeginDrawing();

        player.update(dt, backgroundTexture.width, backgroundTexture.height);
        player.draw();

        Rectangle playerRect = {player.position.x, player.position.y, (float)entityData[PLAYER].texture.width, (float)entityData[PLAYER].texture.height};
        for (size_t i = 0; i < enemies.size(); ++i) {
            if (CheckCollisionRecs(playerRect, {enemies[i].position.x, enemies[i].position.y, (float)entityData[enemies[i].type].texture.width, (float)entityData[enemies[i].type].texture.height})) {
                player.health -= enemies[i].health;
                enemies.erase(enemies.begin()+i);
                --i;
                continue;
            }
            
            enemies[i].draw();
        }

        for (size_t bulletIdx = 0; bulletIdx < bullets.size();) {
            if (handleBullet(bulletIdx, dt, backgroundTexture))
                ++bulletIdx;
        }

        DrawFPS(0, 0);

        consoleUI.draw();
        EndDrawing();
    }

    Meatball::FontsHandler::clear();
    CloseWindow();
}