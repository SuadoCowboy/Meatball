#include "Player.h"

#include "Entity.h"

Player player;

Player::Player() {}

Player::Player(const Vector2& position, const Vector2& speed)
    : position(position), direction({ 1, 1 }), speed(speed) {}

void Player::update(int renderWidth, int renderHeight, float dt) {
    position.x += (((short)direction.x) - 1) * speed.x * renderWidth * dt;
    position.y += (((short)direction.y) - 1) * speed.y * renderHeight * dt;
}

void Player::draw() {
    DrawTexture(entityData[PLAYER].texture, position.x, position.y, WHITE);
}