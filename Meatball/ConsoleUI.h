#pragma once

#include "Scene.h"

#include <raylib.h>

namespace Meatball {
    Scene* createConsoleUI(int x, int y, int width, int height, Color mainPanelColor = { 22, 22, 22, 200 }, bool visible = true);
}