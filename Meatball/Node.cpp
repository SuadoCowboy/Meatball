#include "Node.h"

Meatball::Node::Node(int x, int y, int width, int height, bool visible)
	: x(x), y(y), width(width), height(height), visible(visible) {}

void Meatball::Node::update() {}
void Meatball::Node::draw() {}

bool Meatball::Node::checkCollision(Rectangle& rectangle) {
	Rectangle thisRect = { (float)x, (float)y, (float)width, (float)height };
	return CheckCollisionRecs(thisRect, rectangle);
}

int Meatball::Node::getTypes() {
	return NodeType::NODE;
}