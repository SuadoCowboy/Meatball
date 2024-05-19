#pragma once

#include <raylib.h>

namespace Meatball {
    enum NodeType {
        NODE = 1,

        NODEUI = 2,
        NODEUI_SCENE = 4,
        NODEUI_BUTTON = 8,
        NODEUI_LABEL = 16,
        NODEUI_PANEL = 32 // just a colored rectangle or something
        //NODEUI_BUTTON_TOGGLE = 64,
        //NODEUI_TEXT_INPUT = 128,
        //NODEUI_TEXT_BOX = 256, // or just use text_input and use a variable something related to disable input? or the other way around
        
        //SPRITE = 512, // could be used in UI/2D/3D
        //TEXTURE = 1024
    };

    class Node {
    public:
        Node(float x, float y, float z, float width, float height, bool visible = true);

        virtual void update();
        virtual void draw();

        virtual int getTypes();

        bool visible;
        float x, y, z, width, height;

        virtual bool checkCollision(Rectangle& rectangle);
    };
}