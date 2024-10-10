#include "Enemy.h"

#include "Entity.h"
#include "Bullet.h"

std::vector<Enemy> enemies;

Enemy::Enemy(const Vector2& position, const EntityType& type) : position(position), type(type) {
    health = entityData[type].defaultHealth;
}

bool Enemy::update(Player& player, const Rectangle& playerRect, float dt) {
    if (CheckCollisionRecs(playerRect, { position.x, position.y, (float)entityData[type].texture.width, (float)entityData[type].texture.height })) {
        player.health -= health;
        return true;
    }

    timeSinceLastShot += dt;
    if (CheckCollisionPointRec({position.x+entityData[type].texture.width*0.5f, player.position.y}, playerRect))
        shoot();

    return false;
}

void Enemy::draw() const {
    DrawRectangleLines(position.x, position.y, entityData[type].texture.width, entityData[type].texture.height, {0,0,255,255});
    DrawTexture(entityData[type].texture, position.x, position.y, WHITE);
}

void Enemy::shoot() {
    if (timeSinceLastShot >= shootCooldown) {
        bullets.push_back({ type, { position.x + entityData[type].texture.width * 0.5f - bulletSize.x * 0.5f, position.y + entityData[type].texture.height * 0.5f - bulletSize.y * 0.25f } });
        timeSinceLastShot = 0.0f;
    }
}