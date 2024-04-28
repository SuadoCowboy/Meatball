#pragma once

#include <raylib.h>

namespace Meatball {
    enum NodeType {
        NODE = 1,
        NODEUI = 2,
        SCENE = 4,
        NODEUI_BUTTON = 8,
        NODEUI_LABEL = 16,
        //NODEUI_BUTTON_TOGGLE = 32,
        //NODEUI_PANEL = 64 // just a colored rectangle or something
        //NODEUI_TEXT_INPUT = 128,
        //NODEUI_TEXT_BOX = 256 // or just use text_input and use a variable something related to disable input? or the other way around
        //NODEUI_TEXURE = 512
    };

    class Node {
    public:
        Node(int x = 0, int y = 0, int width = 1, int height = 1, bool visible = true);

        virtual void update();
        virtual void draw();

        virtual int getTypes();

        bool visible;
        int x, y, width, height;

        virtual bool checkCollision(Rectangle& rectangle);
    };
}