#include "Player.h"

#include "Entity.h"

Player player;

Player::Player() {}

Player::Player(const Vector2& position, const Vector2& speed) : position(position), direction({ 0,0 }), speed(speed) {
    direction = { 1, 1 };
    health = 100;
}

void Player::update(float dt, int renderWidth, int renderHeight) {
    position.x += (((short)direction.x) - 1) * dt * speed.x * renderWidth;
    position.y += (((short)direction.y) - 1) * dt * speed.y * renderHeight;
}

void Player::draw() {
    DrawTexture(entityData[PLAYER].texture, position.x, position.y, WHITE);
}