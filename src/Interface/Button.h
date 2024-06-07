#pragma once

#include <string>
#include <memory>

#include <raylib.h>

#include "Shared.h"
#include "Config/Interface.h"

namespace Meatball {
    class Button {
    public:
        Button(std::shared_ptr<Config::Button> config);
        Button(Rectangle &rect, std::shared_ptr<Config::Button> config);
        
        void update();
        bool isHovered();

        VoidFunc onRelease, onHover;
        std::shared_ptr<Config::Button> config;
        Rectangle rect;

    private:
        bool hovered;
    };
}