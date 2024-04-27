#pragma once

#include <raylib.h>

namespace Meatball {
    enum NodeType {
        NODE = 0,
        NODEUI,
        NODEUI_BUTTON,
        //NODEUI_BUTTON_TOGGLE,
        //NODEUI_PANEL // just a colored rectangle or something
        //NODEUI_TEXT_INPUT,
        //NODEUI_TEXT_BOX // or just use text_input and use a variable something related to disable input? or the other way around
        //NODEUI_TEXURE
    };

    class Node {
    public:
        Node(bool visible = true);

        virtual void onMouseButtonPressed();
        virtual void update();
        virtual void draw();

        virtual NodeType getType();

        bool visible;
    };
}