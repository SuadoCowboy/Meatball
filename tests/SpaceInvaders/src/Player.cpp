#include "Player.h"

#include "Entity.h"

Player player;

Player::Player() {}

Player::Player(const Vector2& position, const Vector2& speed)
    : speed(speed) {
        rect = { position.x, position.y, (float)entityData[PLAYER].texture.width, (float)entityData[PLAYER].texture.height };
    }

void Player::update(int renderWidth, int renderHeight, float dt) {
    rect.x += (((short)direction.x) - 1) * speed.x * renderWidth * dt;
    rect.y += (((short)direction.y) - 1) * speed.y * renderHeight * dt;
}

void Player::draw() {
    DrawTexture(entityData[PLAYER].texture, rect.x, rect.y, WHITE);
}