#pragma once

#include "Scene.h"

#include <raylib.h>

namespace Meatball {
    Scene* createConsoleUI(float x, float y, float width, float height, Color mainPanelColor = { 22, 22, 22, 200 }, bool visible = true);
}