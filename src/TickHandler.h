#pragma once

namespace Meatball {
    class TickHandler {
    public:
        TickHandler();
        TickHandler(unsigned char tickRate);
    
        bool shouldTick(const float& dt);
    
        unsigned short getTickInterval();
        void setTickRate(unsigned char tickRate);
    private:
        unsigned short tickInterval = 1000;
        unsigned short delta = 0; // time passed since last tick
    };
}