#include "Label.h"

Meatball::Interface::Label::Label(float fontSize, const Font& font, int x, int y, bool visible, float spacing)
	: NodeUI(x, y, width, height, visible), fontSize(fontSize), font(font), spacing(spacing),
	backgroundColor({ 0, 0, 0, 255 }), foregroundColor({ 255, 255, 255, 255 }) {}

int Meatball::Interface::Label::getTypes() {
	return NodeUI::getTypes() | NodeType::NODEUI_LABEL;
}

void Meatball::Interface::Label::updateSize() {
	Vector2 size = MeasureTextEx(font, text.c_str(), fontSize, spacing);

	width = (int)size.x;
	height = (int)size.y;
}

void Meatball::Interface::Label::setText(const std::string& newText) {
	text = newText;
	updateSize();
}

const std::string& Meatball::Interface::Label::getText() {
	return text;
}

void Meatball::Interface::Label::draw() {
	DrawRectangle(x, y, width, height, backgroundColor);

	Vector2 pos{ (float)x, (float)y };
	DrawTextEx(font, text.c_str(), pos, fontSize, spacing, foregroundColor);
}