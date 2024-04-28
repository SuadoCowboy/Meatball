#pragma once

#include "Node.h"

#include <vector>

namespace Meatball {
	class Scene : public Node {
	public:
		Scene(int x = 0, int y = 0, int width = 1, int height = 1, bool visible = true);
		~Scene();
		
		void handleInput();
		void update();
		void draw();

		virtual int getTypes();

		// adds your Node to the engine so that it can be drawn and updated
		void addNode(Node*);
		//void addNode(Node3D*); // start with 2d nodes and later on try 3d(also check if Node3D class is really needed)
		
	private:
		Node* currentFocusedNode;
		std::vector<Node*> nodes; // for safety purposes, maybe not use index system but rather check by address or something
	};
}