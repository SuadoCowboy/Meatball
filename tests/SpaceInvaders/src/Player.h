#pragma once

#include <raylib.h>

#include "Vector2uc.h"

class Player {
public:
    Player();
    Player(const Vector2& position, const Vector2& speed);

    void update(int renderWidth, int renderHeight, float dt);
    void draw();

    Vector2uc direction = {1,1};
    Vector2 speed = {0.0f,0.0f};
    short health = 100;
    Rectangle rect = {0.0f,0.0f,0.0f,0.0f};
};

extern Player player;