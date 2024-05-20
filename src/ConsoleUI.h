#pragma once

#include "Scene.h"
#include "Interface/Panel.h"

namespace Meatball {
    class ConsoleUIScene : public Scene {
    public:
        ConsoleUIScene(bool visible = true);

        // Only console can appear in every scene so only it needs visible boolean.
        // The rest of the scenes will be handled by a class or something that says which one should be used.
        bool visible;

        Panel mainPanel;
    };
}