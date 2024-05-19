#include "Panel.h"

Meatball::Interface::Panel::Panel(float x, float y, float width, float height, Color color, bool visible)
	: NodeUI(x, y, width, height, visible), color(color) {}

int Meatball::Interface::Panel::getTypes() {
	return NodeUI::getTypes() | NodeType::NODEUI_PANEL;
}

void Meatball::Interface::Panel::draw() {
	DrawRectangle(x, y, width, height, color);
}