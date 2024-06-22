#pragma once

#include <vector>

#include <raylib.h>

#include "Entity.h"

class Enemy {
public:
    Enemy(const Vector2& position, const EntityType& type);
    void draw();
    void shoot();
    
    Vector2 position;
    EntityType type;
    unsigned char damage;
    short health;
};

extern std::vector<Enemy> enemies;