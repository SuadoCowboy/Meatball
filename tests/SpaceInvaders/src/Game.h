#pragma once

#include <raylib.h>

#include <ConsoleUI.h>

Meatball::ConsoleUIScene init(int windowWidth, int windowHeight);

void reloadFonts(Meatball::ConsoleUIScene& consoleUI);
void resize(Meatball::ConsoleUIScene& consoleUI);
/// @brief also updates
void render(Meatball::ConsoleUIScene& consoleUI);
void save(const std::string& path);
void cleanup();

void loadCommands(Meatball::ConsoleUIScene& consoleUI);
bool handleBullet(size_t& bulletIdx);

extern Texture2D backgroundTexture;
extern unsigned char conditionFlags;