#include "Node.h"

Meatball::Node::Node(float x, float y, float z, float width, float height, bool visible)
	: x(x), y(y), z(z), width(width), height(height), visible(visible) {}

void Meatball::Node::update() {}
void Meatball::Node::draw() {}

bool Meatball::Node::checkCollision(Rectangle& rectangle) {
	Rectangle thisRect = { x, y, width, height };
	return CheckCollisionRecs(thisRect, rectangle);
}

int Meatball::Node::getTypes() {
	return NodeType::NODE;
}