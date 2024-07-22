#pragma once

#include <raylib.h>

#include <ConsoleUI.h>

void init(int windowWidth, int windowHeight);

void reloadFonts();
void resize();
/// @brief also updates
void update(const float& dt);
void render();
void save(const std::string& path);
void cleanup();

void loadCommands();
bool handleBullet(size_t& bulletIdx, const float& dt);

extern Meatball::ConsoleUIScene* consoleUI;
extern Texture2D backgroundTexture;
extern unsigned char conditionFlags;