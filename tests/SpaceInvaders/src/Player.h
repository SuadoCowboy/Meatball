#pragma once

#include <raylib.h>

#include "Vector2uc.h"

class Player {
public:
    Player();
    Player(const Vector2& position, const Vector2& speed);

    void update(int renderWidth, int renderHeight);
    void draw();

    Vector2 position;
    Vector2uc direction;
    Vector2 speed;
    short health = 100;
};

extern Player player;