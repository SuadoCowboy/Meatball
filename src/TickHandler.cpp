#include "TickHandler.h"

Meatball::TickHandler::TickHandler(unsigned char tickRate)
    : tickInterval((1.0f / (float)tickRate)*1000) {}

bool Meatball::TickHandler::shouldTick(const float& dt) {
    delta += dt;
    if (delta >= tickInterval) {
        delta = 0;
        return true;
    }
    return false;
}