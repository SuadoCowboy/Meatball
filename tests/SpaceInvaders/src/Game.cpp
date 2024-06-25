#include "Game.h"

#include <string>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>

#include <Console.h>
#include <Utils/Defaults.h>
#include <FontsHandler.h>
#include <MouseCursor.h>
#include <Input.h>
#include <Config.h>
#include <TickHandler.h>

#include "Entity.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

using namespace Meatball;

Meatball::ConsoleUIScene* consoleUI = nullptr;
std::mutex gameMutex;

Texture2D backgroundTexture;

/*
shouldQuit = 1,
saveSettings = 2
*/
unsigned char conditionFlags = 0;

TickHandler tickHandler;

void reloadFonts() {
    std::lock_guard<std::mutex> lock(gameMutex);

    FontsHandler::clear();
    FontsHandler::add(0, GetFontDefault());

    auto consoleData = Config::loadData("data/meatdata/Console.meatdata");

    auto data = Config::ifContainsGet(consoleData, "font");
    std::string path;
    if (data) Defaults::loadConsoleFonts(*consoleUI, ((Config::ConfigTypeData<std::string>*)data)->value);

    Config::clearData(consoleData);
}

void init(int windowWidth, int windowHeight) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(windowWidth, windowHeight, "Space Invaders");
    SetExitKey(KEY_NULL); // disable exit key

    SetWindowMinSize(windowWidth * 0.5, windowHeight * 0.5);
    int currentMonitorId = GetCurrentMonitor();
    SetWindowMaxSize(GetMonitorWidth(currentMonitorId), GetMonitorHeight(currentMonitorId));

    Defaults::init("data/meatdata/Init.meatdata");

    Rectangle consoleUIRect = { 0, 0, windowWidth * 0.5f, windowHeight * 0.75f };
    consoleUIRect.x = windowWidth * 0.5f - consoleUIRect.width * 0.5;
    consoleUIRect.y = windowHeight * 0.5f - consoleUIRect.height * 0.5;

    if (consoleUI != nullptr) delete consoleUI;
    consoleUI = new ConsoleUIScene(Defaults::initLocalConsole(
        consoleUIRect,
        "data/meatdata/Console.meatdata"));

    consoleUI->visible = false;

    entityData[PLAYER] = {10, 90, {255, 255, 0, 255}, LoadTexture("data/images/player.png")};
    entityData[ENEMY_WEAK] = {10, 30, {0, 0, 255, 255}, LoadTexture("data/images/enemy0.png")};
    entityData[ENEMY_MEDIUM] = {25, 60, {255, 0, 0, 255}, LoadTexture("data/images/enemy1.png")};
    entityData[ENEMY_STRONG] = {40, 90, {0, 255, 0, 255}, LoadTexture("data/images/enemy2.png")};

    auto dataMap = Config::loadData("data/meatdata/Game.meatdata");
    auto data = Config::ifContainsGet(dataMap, "playerBulletColor");
    if (data) entityData[PLAYER].color = Config::getConfig<Color>(data)->value;

    data = Config::ifContainsGet(dataMap, "enemy0BulletColor");
    if (data) entityData[ENEMY_WEAK].color = Config::getConfig<Color>(data)->value;

    data = Config::ifContainsGet(dataMap, "enemy1BulletColor");
    if (data) entityData[ENEMY_MEDIUM].color = Config::getConfig<Color>(data)->value;

    data = Config::ifContainsGet(dataMap, "enemy2BulletColor");
    if (data) entityData[ENEMY_STRONG].color = Config::getConfig<Color>(data)->value;

    Config::clearData(dataMap);

    loadCommands();
    Input::registerCommands();
    Input::mapKeyboardKeys();
    Input::mapMouseKeys();

    backgroundTexture = LoadTexture("data/images/background.png");
    backgroundTexture.width = GetRenderWidth();
    backgroundTexture.height = GetRenderHeight();

    player = {{.0f, .0f}, {0.02f, 0.04f}};
    entityData[PLAYER].texture.width = backgroundTexture.width * 0.05f;
    entityData[PLAYER].texture.height = backgroundTexture.width * 0.05f;

    player.position.x = backgroundTexture.width * 0.5f - entityData[PLAYER].texture.width * 0.5f;
    player.position.y = backgroundTexture.height * 0.9f - entityData[PLAYER].texture.height;

    bulletSpeed = 0.02f;

    tickHandler = TickHandler(20);

    conditionFlags |= 2;

    HayBCMD::CVARStorage::setCvar("bullet_speed",
        [](const std::string& str) {
            float value = std::stof(str);
            if (value < 0.0f) return;
            bulletSpeed = value;
        },
        []() { return std::to_string(bulletSpeed); }, "");
    HayBCMD::CVARStorage::setCvar("player_health",
        [](const std::string& value) { player.health = (short)std::stoi(value); },
        []() { return std::to_string(player.health); }, "");

    HayBCMD::CVARStorage::setCvar("player_speed_x",
        [](const std::string& value) { player.speed.x = std::stof(value); },
        []() {
            return std::to_string(player.speed.x);
        }, "");

    HayBCMD::CVARStorage::setCvar("player_speed_y",
        [](const std::string& value) { player.speed.y = std::stof(value); },
        []() {
            return std::to_string(player.speed.y);
        }, "");

    HayBCMD::CVARStorage::setCvar("tick_rate",
        [](const std::string& str) {
            float value = std::stof(str);
            
            if (value <= 0.0f) return;
            
            tickHandler.tickInterval = 1.0f / value;
        },
        [](){ return std::to_string(1.0f/tickHandler.tickInterval); },
        "tick_rate should be between [0, 255]");

    HayBCMD::CVARStorage::setCvar("save_settings",
        [](const std::string& str) {
            bool value = (bool)std::stoi(str);
            if (value) conditionFlags |= 2;
            else conditionFlags &= ~2;
        },
        [](){ return std::to_string((bool)(conditionFlags & 2)); },
        "if should save the changed settings in .meatdata");

    HayBCMD::execConfigFile("data/cfg/autoexec.cfg", Console::variables);
    HayBCMD::execConfigFile("data/cfg/config.cfg", Console::variables);

    enemies = {Enemy({windowWidth * 0.5f, windowHeight * 0.5f}, ENEMY_WEAK)};

    bulletSize.x = backgroundTexture.width * 0.01f;
    bulletSize.y = backgroundTexture.height * 0.04f;
}

void resize() {
    std::lock_guard<std::mutex> lock(gameMutex);

    int newScreenWidth = GetRenderWidth(), newScreenHeight = GetRenderHeight();
    Vector2 ratio = { (float)newScreenWidth / backgroundTexture.width, (float)newScreenHeight / backgroundTexture.height };

    consoleUI->onResize(ratio.x, ratio.y);

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

unsigned short tps = 0;
unsigned char ticks = 0;
float secondCount = 0.0f; // dt + dt + dt + dt + ... = 1.0f = 1 second passed

void update() {
    bool running;
    {
        std::lock_guard<std::mutex> lock(gameMutex);
        running = (conditionFlags & 2);
    }
    while (running) {
        float dt = GetFrameTime();
        secondCount += dt;
        {
            std::lock_guard<std::mutex> lock(gameMutex);
            tickHandler.update(GetFrameTime());
        }
        
        while (tickHandler.shouldTick()) {
            std::lock_guard<std::mutex> lock(gameMutex);

            if (IsWindowResized())
                resize();

            consoleUI->update();
            Input::update(consoleUI->visible);
            
            HayBCMD::handleLoopAliasesRunning(Console::variables);
            if (WindowShouldClose()) conditionFlags |= 1;
            running = (conditionFlags & 2);

            ticks++;
            if (secondCount >= 1.0f) {
                tps = ticks;
                ticks = 0;
                secondCount -= 1.0f;
            }

            player.update(backgroundTexture.width, backgroundTexture.height);

            Rectangle playerRect = { player.position.x, player.position.y, (float)entityData[PLAYER].texture.width, (float)entityData[PLAYER].texture.height };

            for (size_t i = 0; i < enemies.size(); ++i) {
                if (CheckCollisionRecs(playerRect, { enemies[i].position.x, enemies[i].position.y, (float)entityData[enemies[i].type].texture.width, (float)entityData[enemies[i].type].texture.height })) {
                    player.health -= enemies[i].health;
                    enemies.erase(enemies.begin() + i);
                    --i;
                    continue;
                }
            }

            for (size_t bulletIdx = 0; bulletIdx < bullets.size();)
                if (handleBullet(bulletIdx))
                    ++bulletIdx;
        }
    }
}

void render() {
    BeginDrawing();
    
    Vector2 renderSize;
    {
        std::lock_guard<std::mutex> lock(gameMutex);
        renderSize = {(float)backgroundTexture.width, (float)backgroundTexture.height};
        
        DrawTexture(backgroundTexture, 0, 0, WHITE);
    }

    player.draw();

    for (auto& enemy : enemies)
        enemy.draw();

    for (auto& bullet : bullets)
        DrawRectangle(bullet.position.x, bullet.position.y, bulletSize.x, bulletSize.y, entityData[bullet.ownerType].color);
    
    DrawFPS(0, 0);
    DrawText(HayBCMD::formatString("{} TPS", tps).c_str(), 0, 21, 20, LIME);
    consoleUI->draw();

    EndDrawing();
}

void save(const std::string& path) {
    if (!(conditionFlags & 2)) return;

    std::unordered_map<std::string, Meatball::Config::ConfigData*> dataMap;

    dataMap["windowWidth"] = new Meatball::Config::ConfigTypeData(GetScreenWidth());
    dataMap["windowWidth"]->type = Meatball::Config::ConfigType::INT;

    dataMap["windowHeight"] = new Meatball::Config::ConfigTypeData(GetScreenHeight());
    dataMap["windowHeight"]->type = Meatball::Config::ConfigType::INT;

    Meatball::Config::saveData(path, dataMap);
    Meatball::Config::clearData(dataMap);
}

void cleanup() {
    FontsHandler::clear();
    CloseWindow();
}

void loadCommands() {
    HayBCMD::Command("quit", 0, 0, [](void*, const std::vector<std::string>&){ conditionFlags |= 1; }, "- closes the window");
    HayBCMD::Command("+moveup", 0, 0, [](void*, const std::vector<std::string>&) { player.direction.y--; },
        "- moves up");
    HayBCMD::Command("+movedown", 0, 0, [](void*, const std::vector<std::string>&) { player.direction.y++; },
        "- moves down");
    HayBCMD::Command("+moveleft", 0, 0, [](void*, const std::vector<std::string>&) { player.direction.x--; },
        "- moves left");
    HayBCMD::Command("+moveright", 0, 0, [](void*, const std::vector<std::string>&) { player.direction.x++; },
        "- moves right");

    HayBCMD::Command("-moveup", 0, 0, [](void*, const std::vector<std::string>&) { player.direction.y++; },
        "- stops moving up");
    HayBCMD::Command("-movedown", 0, 0, [](void*, const std::vector<std::string>&) { player.direction.y--; },
        "- stops moving down");
    HayBCMD::Command("-moveleft", 0, 0, [](void*, const std::vector<std::string>&) { player.direction.x++; },
        "- stops moving left");
    HayBCMD::Command("-moveright", 0, 0, [](void*, const std::vector<std::string>&) { player.direction.x--; },
        "- stops moving right");

    HayBCMD::Command("+fire", 0, 0, [](void*, const std::vector<std::string>&) {
        bullets.push_back({ PLAYER, { player.position.x + entityData[PLAYER].texture.width * 0.5f - bulletSize.x * 0.5f, player.position.y + bulletSize.y * 0.25f } });
    }, "- shoots a bullet");

    Console::variables["-fire"] = " ";

    HayBCMD::Command("wait", 1, 1, [](void*, const std::vector<std::string>& args) {
        try {
            unsigned int value = (unsigned int)std::stoi(args[0]);
            std::this_thread::sleep_for(std::chrono::milliseconds(value));
        } catch (...) {
            Meatball::Console::print(HayBCMD::ERROR, "value is not a valid integer or duration type");
            return;
        }
    }, "- waits in milliseconds");

    HayBCMD::Command("reload_fonts", 0, 0, [&](void*, const std::vector<std::string>&) {
        reloadFonts();
    }, "- reloads all text fonts.");

    HayBCMD::Command("toggle_local_console", 0, 0, [&](void*, const std::vector<std::string>&) {
        consoleUI->visible = not consoleUI->visible;
        if (!consoleUI->visible)
            resetCursor(MouseCursorPriorityLevel::INPUT_TEXT_BOX);
    }, "- toggles the console ui visibility");

    Input::allowedUiCommands.push_back("toggle_local_console");
    Input::allowedUiCommands.push_back("quit");
}

bool handleBullet(size_t& bulletIdx) {
    Rectangle rect = { bullets[bulletIdx].position.x, bullets[bulletIdx].position.y, bulletSize.x, bulletSize.y };
    float movement = (bulletSpeed * backgroundTexture.height);
    
    unsigned int substeps = std::ceil(movement / bulletSize.y);
    float substepMovement = movement / substeps;
    for (unsigned int i = 0; i < substeps; ++i) {
        rect.y += substepMovement * (bullets[bulletIdx].ownerType == PLAYER ? -1 : 1);

        if (bullets[bulletIdx].position.y > backgroundTexture.height || bullets[bulletIdx].position.y + bulletSize.y < 0) {
            bullets.erase(bullets.begin() + bulletIdx);
            return false;
        }

        if (bullets[bulletIdx].ownerType == PLAYER) {
            bool hitEnemy = false;

            for (size_t enemyIdx = 0; enemyIdx < enemies.size(); ++enemyIdx) {
                if (!CheckCollisionRecs(rect, { enemies[enemyIdx].position.x, enemies[enemyIdx].position.y, (float)entityData[enemies[enemyIdx].type].texture.width, (float)entityData[enemies[enemyIdx].type].texture.height }))
                    continue;

                enemies[enemyIdx].health -= entityData[bullets[bulletIdx].ownerType].damage;
                hitEnemy = true;

                if (enemies[enemyIdx].health <= 0) {
                    enemies.erase(enemies.begin() + enemyIdx);
                    --enemyIdx;
                }
                break;
            }

            if (hitEnemy) {
                bullets.erase(bullets.begin() + bulletIdx);
                return false;
            }
        }
        else {
            if (CheckCollisionRecs(rect, { player.position.x, player.position.y, (float)entityData[PLAYER].texture.width, (float)entityData[PLAYER].texture.height })) {
                player.health -= entityData[bullets[bulletIdx].ownerType].damage;
                bullets.erase(bullets.begin() + bulletIdx);
                return false;
            }
        }
    }

    bullets[bulletIdx].position.y = rect.y;
    return true;
}