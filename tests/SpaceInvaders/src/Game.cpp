#include "Game.h"

#include <string>
#include <vector>
#include <cmath>

#include <Console.h>
#include <Utils/Defaults.h>
#include <FontsHandler.h>
#include <MouseCursor.h>
#include <Input.h>
#include <Config.h>

#include "Entity.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

bool shouldQuit = false;

using namespace Meatball;

Texture2D backgroundTexture;

ConsoleUIScene init(int windowWidth, int windowHeight) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(windowWidth, windowHeight, "Console Build In Progress");
    SetExitKey(KEY_NULL); // disable exit key

    SetWindowMinSize(windowWidth * 0.5, windowHeight * 0.5);
    int currentMonitorId = GetCurrentMonitor();
    SetWindowMaxSize(GetMonitorWidth(currentMonitorId), GetMonitorHeight(currentMonitorId));

    Defaults::init("data/meatdata/Init.meatdata");

    Rectangle consoleUIRect = { 0, 0, windowWidth * 0.5f, windowHeight * 0.75f };
    consoleUIRect.x = windowWidth * 0.5f - consoleUIRect.width * 0.5;
    consoleUIRect.y = windowHeight * 0.5f - consoleUIRect.height * 0.5;

    auto consoleUI = Defaults::initLocalConsole(
        consoleUIRect,
        "data/meatdata/Console.meatdata");

    consoleUI.visible = false;

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

    loadCommands(consoleUI);
    Input::registerCommands();
    Input::mapKeyboardKeys();
    Input::mapMouseKeys();

    backgroundTexture = LoadTexture("data/images/background.png");
    backgroundTexture.width = GetRenderWidth();
    backgroundTexture.height = GetRenderHeight();

    player = {{.0f, .0f}, {0.4f, 0.6f}};
    entityData[PLAYER].texture.width = backgroundTexture.width * 0.05f;
    entityData[PLAYER].texture.height = backgroundTexture.width * 0.05f;

    player.position.x = backgroundTexture.width * 0.5f - entityData[PLAYER].texture.width * 0.5f;
    player.position.y = backgroundTexture.height * 0.9f - entityData[PLAYER].texture.height;

    bulletSpeed = 0.6f;

    HayBCMD::CVARStorage::setCvar("bullet_speed",
        [](const std::string& value) {
            bulletSpeed = std::stof(value);
            if (bulletSpeed < 0.0f)
                bulletSpeed = 0.0f;
        },
        []() { return std::to_string(bulletSpeed); }, "");
    HayBCMD::CVARStorage::setCvar("player_health",
        [](const std::string& value) { player.health = (unsigned char)std::stoi(value); },
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

    HayBCMD::execConfigFile("data/cfg/autoexec.cfg", Console::variables);
    HayBCMD::execConfigFile("data/cfg/config.cfg", Console::variables);

    enemies = {Enemy({windowWidth * 0.5f, windowHeight * 0.5f}, ENEMY_WEAK)};

    bulletSize.x = backgroundTexture.width * 0.01f;
    bulletSize.y = backgroundTexture.height * 0.04f;

    return consoleUI;
}

void resize(ConsoleUIScene& consoleUI) {
    int newScreenWidth = GetRenderWidth(), newScreenHeight = GetRenderHeight();
    Vector2 ratio = { (float)newScreenWidth / backgroundTexture.width, (float)newScreenHeight / backgroundTexture.height };

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

void render(ConsoleUIScene& consoleUI) {
    ClearBackground(BLACK);
    DrawTexture(backgroundTexture, 0, 0, WHITE);

    float dt = GetFrameTime();

    consoleUI.update();
    Input::update(consoleUI.visible);
    HayBCMD::handleLoopAliasesRunning(Console::variables);
    if (WindowShouldClose()) shouldQuit = true;

    player.update(dt, backgroundTexture.width, backgroundTexture.height);

    BeginDrawing();

    player.draw();

    Rectangle playerRect = { player.position.x, player.position.y, (float)entityData[PLAYER].texture.width, (float)entityData[PLAYER].texture.height };
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (CheckCollisionRecs(playerRect, { enemies[i].position.x, enemies[i].position.y, (float)entityData[enemies[i].type].texture.width, (float)entityData[enemies[i].type].texture.height })) {
            player.health -= enemies[i].health;
            enemies.erase(enemies.begin() + i);
            --i;
            continue;
        }

        enemies[i].draw();
    }

    for (size_t bulletIdx = 0; bulletIdx < bullets.size();) {
        if (handleBullet(bulletIdx, dt))
            ++bulletIdx;
    }

    DrawFPS(0, 0);
    consoleUI.draw();

    EndDrawing();
}

void cleanup() {
    FontsHandler::clear();
    CloseWindow();
}

void loadCommands(ConsoleUIScene& consoleUI) {
    HayBCMD::Command("quit", 0, 0, [](void*, const std::vector<std::string>&){ shouldQuit = true; }, "- closes the window");
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

    HayBCMD::Command("reload_fonts", 0, 0, [&](void*, const std::vector<std::string>&) {
        FontsHandler::clear();
        FontsHandler::add(0, GetFontDefault());

        auto consoleData = Config::loadData("data/meatdata/Console.meatdata");

        auto data = Config::ifContainsGet(consoleData, "font");
        std::string path;
        if (data) Defaults::loadConsoleFonts(consoleUI, ((Config::ConfigTypeData<std::string>*)data)->value);

        Config::clearData(consoleData);
    }, "- reloads all text fonts.");

    HayBCMD::Command("toggle_local_console", 0, 0, [&](void*, const std::vector<std::string>&) {
        consoleUI.visible = not consoleUI.visible;
        if (!consoleUI.visible)
            resetCursor(MouseCursorPriorityLevel::INPUT_TEXT_BOX);
    }, "- toggles the console ui visibility");

    Input::allowedUiCommands.push_back("toggle_local_console");
    Input::allowedUiCommands.push_back("quit");
}

bool handleBullet(size_t& bulletIdx, const float& dt) {
    Rectangle rect = { bullets[bulletIdx].position.x, bullets[bulletIdx].position.y, bulletSize.x, bulletSize.y };
    float movement = (bulletSpeed * backgroundTexture.height * dt);
    
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
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, entityData[bullets[bulletIdx].ownerType].color);
    return true;
}