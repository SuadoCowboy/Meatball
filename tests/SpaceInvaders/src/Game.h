#pragma once

#include <raylib.h>

#include <ConsoleUI.h>

void init(int windowWidth, int windowHeight);

void reloadFonts();
void resize();
/// @brief also updates
void render(float dt);
void save(const std::string& path);
void cleanup();

void loadCommands();
bool handleBullet(size_t bulletIdx, const Rectangle& playerRect, float dt);

extern Meatball::ConsoleUI* pConsoleUI;
extern Texture2D backgroundTexture;
extern bool shouldQuit;
extern bool saveSettings;