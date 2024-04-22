#pragma once

namespace Meatball {
    class NodeUI {
    public:
        NodeUI();
        NodeUI(bool visible);

        virtual void update();
        virtual void draw();
        virtual void handleInput();

        // events
        virtual void onFocusGain();
        virtual void onFocusLoss();

        bool visible;
    };
}