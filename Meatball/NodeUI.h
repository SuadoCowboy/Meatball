#pragma once

#include "Node.h"

#include <functional>

namespace Meatball {
    class Scene;
    
    namespace Interface {
        class NodeUI : public Node {
        public:
            NodeUI(int x, int y, int width, int height, bool visible = true);

            // events
            void onFocusGain(Scene& scene);
            void onFocusLoss(Scene& scene);
            
            virtual void update();

            // sets anchor to another node
            void setAnchor(NodeUI* node);

            virtual int getTypes();
            
            void connectOnFocusGain(const std::function<void(Scene&, NodeUI&)>& lambda);
            void connectOnFocusLoss(const std::function<void(Scene&, NodeUI&)>& lambda);

            // completely ignores input and if a object is behind it, it passes to that object
            bool inputPassThrough;
        private:
            int relativeX, relativeY; // for anchor mode
            NodeUI* nodeAnchoredTo;

            std::function<void(Scene&, NodeUI&)> onFocusGainConnector;
            std::function<void(Scene&, NodeUI&)> onFocusLossConnector;
	    };
    }
}