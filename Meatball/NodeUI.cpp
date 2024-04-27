#include "NodeUI.h"

Meatball::Interface::NodeUI::NodeUI(int x, int y, int width, int height, bool visible)
	: Node(visible), x(x), y(y), width(width), height(height), nodeAnchoredTo(nullptr), relativeX(0), relativeY(0) {}

void Meatball::Interface::NodeUI::onFocusGain() {}
void Meatball::Interface::NodeUI::onFocusLoss() {}

Meatball::NodeType Meatball::Interface::NodeUI::getType() {
	return NodeType::NODEUI;
}

void Meatball::Interface::NodeUI::updatePosition() {
	if (nodeAnchoredTo == nullptr)
		return;

	x = nodeAnchoredTo->x + relativeX;
	y = nodeAnchoredTo->y + relativeY;
}

void Meatball::Interface::NodeUI::setAnchor(NodeUI* node) {
	//if (node == this) // come on, just don't do that stupidity, no need to put a if statement for people, right?
	//	return;

	nodeAnchoredTo = node;
	if (nodeAnchoredTo == nullptr) {
		relativeX = 0;
		relativeY = 0;
		return;
	}

	relativeX = x - nodeAnchoredTo->x;
	relativeY = y - nodeAnchoredTo->y;
}