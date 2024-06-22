#pragma once

#include <unordered_map>

#include <raylib.h>

enum EntityType {
    PLAYER = 0,
    ENEMY_WEAK,
    ENEMY_MEDIUM,
    ENEMY_STRONG
};

struct EntityData {
    unsigned char damage;
    unsigned char defaultHealth;
    Color color;
    Texture2D texture;
};

extern std::unordered_map<EntityType, EntityData> entityData;