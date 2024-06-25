#pragma once

#include <mutex>

#include <raylib.h>

#include <ConsoleUI.h>

void init(int windowWidth, int windowHeight);

void reloadFonts();
void resize();
/// @brief also updates
void update();
void render();
void save(const std::string& path);
void cleanup();

void loadCommands();
bool handleBullet(size_t& bulletIdx);

extern Meatball::ConsoleUIScene* consoleUI;
extern std::mutex gameMutex;
extern Texture2D backgroundTexture;
extern unsigned char conditionFlags;