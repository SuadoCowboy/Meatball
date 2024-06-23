#pragma once

namespace Meatball {
    class TickHandler {
    public:
        TickHandler();
        TickHandler(float tickRate);

        void update(const float& dt);
        bool shouldTick();
        
        float tickInterval = 1.0f;
    
    private:
        float delta = 0.0f; // time passed since last tick
    };
}