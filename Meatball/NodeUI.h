#pragma once

#include "Node.h"

namespace Meatball {
    namespace Interface {
	    class NodeUI : public Node {
        public:
            NodeUI(int x = 0, int y = 0, int width = 1, int height = 1, bool visible = true);

            // events
            virtual void onFocusGain();
            virtual void onFocusLoss();

            // sets anchor to another node
            void setAnchor(NodeUI* node);
            // position relative to anchor
            void updatePosition();

            virtual NodeType getType();

            int x, y, width, height;

        private:
            int relativeX, relativeY; // for anchor mode
            NodeUI* nodeAnchoredTo;
	    };
    }
}