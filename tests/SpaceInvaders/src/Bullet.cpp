#include "Bullet.h"

std::vector<Bullet> bullets;
float bulletSpeed;
Vector2 bulletSize;

Bullet::Bullet(const EntityType& ownerType, const Vector2& position)
        : ownerType(ownerType), position(position) {}