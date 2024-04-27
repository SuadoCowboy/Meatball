#include "Node.h"

Meatball::Node::Node(bool visible) : visible(visible) {}

void Meatball::Node::onMouseButtonPressed() {}
void Meatball::Node::update() {}
void Meatball::Node::draw() {}

Meatball::NodeType Meatball::Node::getType() {
	return NodeType::NODE;
}