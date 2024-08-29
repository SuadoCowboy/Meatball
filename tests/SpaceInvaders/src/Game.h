#pragma once

#include <raylib.h>

#include <ConsoleUI.h>

void init(int windowWidth, int windowHeight);

void reloadFonts();
void resize();
/// @brief also updates
void update(float dt);
void render();
void save(const std::string& path);
void cleanup();

void loadCommands();
bool handleBullet(size_t& bulletIdx, float dt);

extern Meatball::ConsoleUI* pConsoleUI;
extern Texture2D backgroundTexture;
extern bool shouldQuit;
extern bool saveSettings;