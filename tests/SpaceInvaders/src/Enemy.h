#pragma once

#include <vector>

#include <raylib.h>

#include "Entity.h"
#include "Player.h"

class Enemy {
public:
    Enemy(const Vector2& position, const EntityType& type);

    /// @return if should delete or not from enemies vector 
    bool update(Player& player, const Rectangle& playerRect, float dt);
    void draw() const;
    void shoot();
    
    Vector2 position;
    EntityType type;
    unsigned char damage;
    short health;
    float shootCooldown = 0.5f;
    float timeSinceLastShot = shootCooldown; 
};

extern std::vector<Enemy> enemies;