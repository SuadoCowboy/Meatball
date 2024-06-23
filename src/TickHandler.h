#pragma once

namespace Meatball {
    class TickHandler {
    public:
        TickHandler(unsigned char tickRate);
    
        bool shouldTick(const float& dt);
    
    private:
        const unsigned short tickInterval;
        float delta = 0.0f; // time passed until 1
    };
}