#pragma once

namespace Meatball::Time {
    class Tick {
    public:
        Tick(unsigned char tickRate);
    
        bool shouldTick(const float& dt);
    private:
        const unsigned short tickInterval;
        float delta = 0.0f; // time passed until 1
    };
}