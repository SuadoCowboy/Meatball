#include "NodeUI.h"

Meatball::Interface::NodeUI::NodeUI(int x, int y, int width, int height, bool visible)
	: Node(x, y, width, height, visible), nodeAnchoredTo(nullptr), relativeX(0), relativeY(0) {}

void Meatball::Interface::NodeUI::onFocusGain() {
	if (onFocusGainConnector) onFocusGainConnector(*this);
}

void Meatball::Interface::NodeUI::onFocusLoss() {
	if (onFocusLossConnector) onFocusLossConnector(*this);
}

void Meatball::Interface::NodeUI::update() {
	// updates position relative to anchor
	if (nodeAnchoredTo == nullptr)
		return;

	x = nodeAnchoredTo->x + relativeX;
	y = nodeAnchoredTo->y + relativeY;
}

void Meatball::Interface::NodeUI::connectOnFocusGain(const std::function<void(NodeUI&)>& lambda) {
	onFocusGainConnector = lambda;
}

void Meatball::Interface::NodeUI::connectOnFocusLoss(const std::function<void(NodeUI&)>& lambda) {
	onFocusLossConnector = lambda;
}

int Meatball::Interface::NodeUI::getTypes() {
	return Node::getTypes() | NodeType::NODEUI;
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