#include "Panel.h"

Meatball::Interface::Panel::Panel(int x, int y, int width, int height, Color color, bool visible)
	: NodeUI(x, y, width, height, visible), color(color) {}

int Meatball::Interface::Panel::getTypes() {
	return NodeUI::getTypes() | NodeType::NODEUI_PANEL;
}

void Meatball::Interface::Panel::draw() {
	DrawRectangle(x, y, width, height, color);
}