#pragma once

namespace Meatball {
    class NodeUI {
    public:
        NodeUI();
        NodeUI(bool visible);

        virtual void handleInput();
        virtual void update();
        virtual void draw();

        // events
        virtual void onFocusGain();
        virtual void onFocusLoss();

        bool visible;
    };
}