#include "Game.h"

#include <string>
#include <vector>
#include <cmath>

#include <Console.h>
#include <Utils/Defaults.h>
#include <MouseCursor.h>
#include <Input.h>
#include <Config.h>

#include "Entity.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

using namespace Meatball;

Meatball::ConsoleUI* pConsoleUI;

struct ConsoleFonts {
    Font label;
    Font inputBox;
    Font outputBox;
};
ConsoleFonts consoleFonts;

Texture2D backgroundTexture;

bool shouldQuit = false;
bool saveSettings = true;

Font consoleGeneralFont, consoleLabelFont;

#pragma region commands
static void quitCommand(void*, HayBCMD::Command&, const std::vector<std::string>&) {
    shouldQuit = true;
}

static void moveUpCommand(void*, HayBCMD::Command&, const std::vector<std::string>&) {
    player.direction.y--;
}

static void moveDownCommand(void*, HayBCMD::Command&, const std::vector<std::string>&) {
    player.direction.y++;
}

static void moveLeftCommand(void*, HayBCMD::Command&, const std::vector<std::string>&) {
    player.direction.x--;
}

static void moveRightCommand(void*, HayBCMD::Command&, const std::vector<std::string>&) {
    player.direction.x++;
}

static void fireCommand(void*, HayBCMD::Command&, const std::vector<std::string>&) {
    bullets.push_back({ PLAYER, { player.position.x + entityData[PLAYER].texture.width * 0.5f - bulletSize.x * 0.5f, player.position.y + bulletSize.y * 0.25f } });
}

static void reloadFontsCommand(void*, HayBCMD::Command&, const std::vector<std::string>&) {
    reloadFonts();
}

static void toggleLocalConsoleCommand(void*, HayBCMD::Command&, const std::vector<std::string>&) {
    pConsoleUI->visible = not pConsoleUI->visible;
    if (!pConsoleUI->visible)
        resetCursor(currentMouseCursorPriorityLevel);
}

#pragma endregion

void init(int windowWidth, int windowHeight) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(windowWidth, windowHeight, "Space Invaders");
    SetExitKey(KEY_NULL); // disable exit key

    SetWindowMinSize(windowWidth * 0.5, windowHeight * 0.5);
    int currentMonitorId = GetCurrentMonitor();
    SetWindowMaxSize(GetMonitorWidth(currentMonitorId), GetMonitorHeight(currentMonitorId));

    Font defaultFont = GetFontDefault();
    Defaults::init("data/meatdata/Init.meatdata", defaultFont);

    Rectangle consoleUIRect = { 0, 0, windowWidth * 0.5f, windowHeight * 0.75f };
    consoleUIRect.x = windowWidth * 0.5f - consoleUIRect.width * 0.5f;
    consoleUIRect.y = windowHeight * 0.5f - consoleUIRect.height * 0.5f;

    pConsoleUI = new ConsoleUI(Defaults::initLocalConsole(
        consoleUIRect,
        "data/meatdata/Console.meatdata",
        consoleGeneralFont,
        consoleLabelFont));

    pConsoleUI->visible = false;

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

    player = {{.0f, .0f}, {0.6f, 0.8f}};
    entityData[PLAYER].texture.width = backgroundTexture.width * 0.05f;
    entityData[PLAYER].texture.height = backgroundTexture.width * 0.05f;

    player.position.x = backgroundTexture.width * 0.5f - entityData[PLAYER].texture.width * 0.5f;
    player.position.y = backgroundTexture.height * 0.9f - entityData[PLAYER].texture.height;

    bulletSpeed = 0.8f;

    HayBCMD::CVARStorage::setCvar("bullet_speed",
        &bulletSpeed,
        HayBCMD::CVARUtils::setFloat,
        HayBCMD::CVARUtils::getFloat,
        "");
    HayBCMD::CVARStorage::setCvar("player_health",
        &player.health,
        HayBCMD::CVARUtils::setShort,
        HayBCMD::CVARUtils::getShort,
        "");

    HayBCMD::CVARStorage::setCvar("player_speed_x",
        &player.speed.x,
        HayBCMD::CVARUtils::setFloat,
        HayBCMD::CVARUtils::getFloat,
        "");

    HayBCMD::CVARStorage::setCvar("player_speed_y",
        &player.speed.y,
        HayBCMD::CVARUtils::setFloat,
        HayBCMD::CVARUtils::getFloat,
        "");

    HayBCMD::CVARStorage::setCvar("save_settings",
        &saveSettings,
        HayBCMD::CVARUtils::setBoolean,
        HayBCMD::CVARUtils::getBoolean,
        "if should save the changed settings in .meatdata");

    HayBCMD::execConfigFile("data/cfg/autoexec.cfg", Console::variables);
    HayBCMD::execConfigFile("data/cfg/config.cfg", Console::variables);

    enemies = {Enemy({windowWidth * 0.5f, windowHeight * 0.5f}, ENEMY_WEAK)};

    bulletSize.x = backgroundTexture.width * 0.01f;
    bulletSize.y = backgroundTexture.height * 0.04f;
}

void reloadFonts() {
    auto consoleData = Config::loadData("data/meatdata/Console.meatdata");

    auto data = Config::ifContainsGet(consoleData, "font");
    std::string path;
    if (data) Defaults::loadConsoleFonts(*pConsoleUI, ((Config::ConfigTypeData<std::string>*)data)->value, consoleGeneralFont, consoleLabelFont);

    Config::clearData(consoleData);
}

void resize() {
    int newScreenWidth = GetRenderWidth(), newScreenHeight = GetRenderHeight();
    Vector2 ratio = { (float)newScreenWidth / backgroundTexture.width, (float)newScreenHeight / backgroundTexture.height };

    pConsoleUI->onResize(ratio.x, ratio.y);

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

void update(float dt) {
    pConsoleUI->update();
    Input::update(pConsoleUI->visible);

    if (WindowShouldClose()) shouldQuit = true;
    if (IsWindowResized())
        resize();

    HayBCMD::handleLoopAliasesRunning(Console::variables);

    player.update(backgroundTexture.width, backgroundTexture.height, dt);

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
        if (handleBullet(bulletIdx, dt))
            ++bulletIdx;
}

void render() {
    BeginDrawing();
    DrawTexture(backgroundTexture, 0, 0, WHITE);

    for (auto& bullet : bullets)
        DrawRectangle(bullet.position.x, bullet.position.y, bulletSize.x, bulletSize.y, entityData[bullet.ownerType].color);

    for (auto& enemy : enemies)
        enemy.draw();

    player.draw();
    
    DrawFPS(0, 0);
    pConsoleUI->draw();

    EndDrawing();
}

void save(const std::string& path) {
    if (!saveSettings) return;

    std::unordered_map<std::string, Meatball::Config::ConfigData*> dataMap;

    dataMap["windowWidth"] = new Meatball::Config::ConfigTypeData(GetScreenWidth());
    dataMap["windowWidth"]->type = Meatball::Config::ConfigType::INT;

    dataMap["windowHeight"] = new Meatball::Config::ConfigTypeData(GetScreenHeight());
    dataMap["windowHeight"]->type = Meatball::Config::ConfigType::INT;

    Meatball::Config::saveData(path, dataMap);
    Meatball::Config::clearData(dataMap);
}

void cleanup() {
    UnloadFont(consoleFonts.label);
    UnloadFont(consoleFonts.inputBox);
    UnloadFont(consoleFonts.outputBox);
    CloseWindow();

    delete pConsoleUI;
    pConsoleUI = nullptr;
}

void loadCommands() {
    HayBCMD::Command("quit", 0, 0, quitCommand, "- closes the window");
    HayBCMD::Command("+moveup", 0, 0, moveUpCommand, "- moves up");
    HayBCMD::Command("+movedown", 0, 0, moveDownCommand, "- moves down");
    HayBCMD::Command("+moveleft", 0, 0, moveLeftCommand, "- moves left");
    HayBCMD::Command("+moveright", 0, 0, moveRightCommand, "- moves right");

    HayBCMD::Command("-moveup", 0, 0, moveDownCommand, "- stops moving up");
    HayBCMD::Command("-movedown", 0, 0, moveUpCommand, "- stops moving down");
    HayBCMD::Command("-moveleft", 0, 0, moveRightCommand, "- stops moving left");
    HayBCMD::Command("-moveright", 0, 0, moveLeftCommand, "- stops moving right");

    HayBCMD::Command("+fire", 0, 0, fireCommand, "- shoots a bullet");

    Console::variables["-fire"] = " ";

    HayBCMD::Command("reload_fonts", 0, 0, reloadFontsCommand, "- reloads all text fonts.");

    HayBCMD::Command("toggle_local_console", 0, 0, toggleLocalConsoleCommand, "- toggles the console ui visibility");

    Input::allowedUiCommands.push_back("toggle_local_console");
    Input::allowedUiCommands.push_back("quit");
}

bool handleBullet(size_t& bulletIdx, float dt) {
    Rectangle rect = { bullets[bulletIdx].position.x, bullets[bulletIdx].position.y, bulletSize.x, bulletSize.y };
    float movement = bulletSpeed * backgroundTexture.height * dt;
    
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