#include "Enemy.h"

#include "Entity.h"
#include "Bullet.h"

std::vector<Enemy> enemies;

Enemy::Enemy(const Vector2& position, const EntityType& type) : position(position), type(type) {
    health = entityData[type].defaultHealth;
}

void Enemy::draw() {
    DrawTexture(entityData[type].texture, position.x, position.y, WHITE);
}

void Enemy::shoot() {
    bullets.push_back({ type, { position.x + entityData[type].texture.width * 0.5f - bulletSize.x * 0.5f, position.y + entityData[type].texture.height * 0.5f - bulletSize.y * 0.25f } });
}