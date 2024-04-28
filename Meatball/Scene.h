#pragma once

#include "NodeUI.h"

#include <vector>

namespace Meatball {
	class Scene : public Interface::NodeUI {
	public:
		Scene(int x, int y, int width, int height, bool visible = true);
		~Scene();
		
		void handleInput();
		void update();
		void draw();

		virtual int getTypes();

		// adds your Node to the engine so that it can be drawn and updated
		void addNode(Interface::NodeUI*);
		//void addNode(Node2D*);
		//void addNode(Node3D*); // start with 2d nodes and later on try 3d(also check if Node3D class is really needed)
		
		// function used to sort by layers system (which use z variable)
		// SHOULD BE USED!
		void sortUINodesVector();

	private:
		Interface::NodeUI* currentFocusedUINode; // only required for ui nodes
		std::vector<Interface::NodeUI*> uiNodes; // for safety purposes, maybe not use index system but rather check by address or something
		
		//std::vector<Node*> nodes; // any other node that is not UI\2D\3D (probably not a required vector) 
		//std::vector<Node2D*> 2dNodes; 
		//std::vector<Node3D*> 3dNodes; 
	};
}