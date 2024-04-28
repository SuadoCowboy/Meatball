#pragma once

#include "Node.h"

#include <functional>

namespace Meatball {
    namespace Interface {
	    class NodeUI : public Node {
        public:
            NodeUI(int x = 0, int y = 0, int width = 1, int height = 1, bool visible = true);

            // events
            void onFocusGain();
            void onFocusLoss();
            
            virtual void update();

            // sets anchor to another node
            void setAnchor(NodeUI* node);

            virtual int getTypes();
            
            void connectOnFocusGain(const std::function<void(NodeUI&)>& lambda);
            void connectOnFocusLoss(const std::function<void(NodeUI&)>& lambda);

        private:
            int relativeX, relativeY; // for anchor mode
            NodeUI* nodeAnchoredTo;

            std::function<void(NodeUI&)> onFocusGainConnector;
            std::function<void(NodeUI&)> onFocusLossConnector;
	    };
    }
}