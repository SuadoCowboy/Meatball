#include "TickHandler.h"

Meatball::TickHandler::TickHandler() {}

Meatball::TickHandler::TickHandler(float tickRate) {
    tickInterval = 1.0f / tickRate;
}

void Meatball::TickHandler::update(const float& dt) {
    delta += dt;
}

bool Meatball::TickHandler::shouldTick() {
    if (delta >= tickInterval) {
        delta -= tickInterval;
        return true;
    }
    return false;
}