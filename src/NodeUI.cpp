#include "NodeUI.h"

#include "Console.h"

Meatball::Interface::NodeUI::NodeUI(float x, float y, float width, float height, bool visible)
	: Node(x, y, 0, width, height, visible), nodeAnchoredTo(nullptr),
	anchorRelative({ 0, 0 }), inputPassThrough(false) {}

void Meatball::Interface::NodeUI::onFocusGain(Scene& scene) {
	if (onFocusGainConnector) onFocusGainConnector(scene, *this);
}

void Meatball::Interface::NodeUI::onFocusLoss(Scene& scene) {
	if (onFocusLossConnector)
		onFocusLossConnector(scene, *this);
}

void Meatball::Interface::NodeUI::update() {
	// updates position relative to anchor
	if (nodeAnchoredTo == nullptr)
		return;

	x = nodeAnchoredTo->x + anchorRelative.x;
	y = nodeAnchoredTo->y + anchorRelative.y;
}

Meatball::Interface::NodeUI* Meatball::Interface::NodeUI::getAnchor() {
	return nodeAnchoredTo;
}

void Meatball::Interface::NodeUI::connectOnFocusGain(const std::function<void(Scene&, NodeUI&)>& lambda) {
	onFocusGainConnector = lambda;
}

void Meatball::Interface::NodeUI::connectOnFocusLoss(const std::function<void(Scene&, NodeUI&)>& lambda) {
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
		anchorRelative.x = 0;
		anchorRelative.y = 0;
		return;
	}

	anchorRelative.x = x - nodeAnchoredTo->x;
	anchorRelative.y = y - nodeAnchoredTo->y;
}