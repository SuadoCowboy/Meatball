#pragma once

#include "NodeUI.h"

#include <vector>

namespace Meatball {
	class Scene {
	public:
		Scene();
		~Scene();

		void handleInput();
		void update();
		void draw();

		// adds your Node to the engine so that it can be drawn and updated
		void addNode(NodeUI*);
		//void addNode(Node2D*);
		//void addNode(Node3D*); // start with 2d nodes and after try 3d
	private:
		size_t currentFocusedUINodeIndex; // index value in uiNodes vector
		std::vector<NodeUI*> uiNodes;
	};
}