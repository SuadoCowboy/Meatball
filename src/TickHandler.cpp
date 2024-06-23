#include "TickHandler.h"

Meatball::TickHandler::TickHandler() {}

Meatball::TickHandler::TickHandler(unsigned char tickRate) {
    setTickRate(tickRate);
}

unsigned short Meatball::TickHandler::getTickInterval() {
    return tickInterval;
}

void Meatball::TickHandler::setTickRate(unsigned char tickRate) {
    tickInterval = (1.0f / (float)tickRate)*1000.0f;
}

bool Meatball::TickHandler::shouldTick(const float& dt) {
    delta += (unsigned short)(dt*1000);
    if (delta >= tickInterval) {
        delta = 0;
        return true;
    }
    return false;
}