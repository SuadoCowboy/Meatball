#pragma once

#include <vector>

#include <raylib.h>

#include "Entity.h"

struct Bullet {
    EntityType ownerType;
    Vector2 position;

    Bullet(const EntityType& ownerType, const Vector2& position);
};

extern std::vector<Bullet> bullets;
extern float bulletSpeed;
extern Vector2 bulletSize;