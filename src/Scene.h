#pragma once

#include <raylib.h>

#include "NodeUI.h"

#include <vector>

namespace Meatball {
	class Scene {
	public:
		Scene(bool visible = true);
		~Scene();
		
		void handleInput();
		void update();
		void draw();

		// adds your Node to the engine so that it can be drawn and updated
		void addNode(Interface::NodeUI*);
		//void addNode(Node2D*);
		//void addNode(Node3D*); // start with 2d nodes and later on try 3d(also check if Node3D class is really needed)
		
		const std::vector<Interface::NodeUI*>& getUINodes();
		//const std::vector<Interface::Node2D*>& get2DNodes();
		//const std::vector<Interface::Node3D*>& get3DNodes();

		Interface::NodeUI* getCurrentFocusedUINode();
		
		// function used to sort by layers system (which use z variable)
		// SHOULD BE USED!
		void sortUINodesVector();

		bool visible;

	private:
		Interface::NodeUI* currentFocusedUINode; // only required for ui nodes
		std::vector<Interface::NodeUI*> uiNodes; // for safety purposes, maybe not use index system but rather check by address or something

		//std::vector<Node2D*> 2dNodes;
		//std::vector<Node3D*> 3dNodes;
	};
}