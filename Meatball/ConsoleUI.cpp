#include "ConsoleUI.h"

#include "Console.h"

Meatball::ConsoleUI::ConsoleUI(float x, float y, float width, float height, Color color) : NodeUI(true) {
	mainPanel = {
		.x = x,
		.y = y,
		.width = width,
		.height = height
	};

	mainPanelColor = color;
}

Meatball::ConsoleUI::~ConsoleUI() {}

void Meatball::ConsoleUI::handleInput() {}

void Meatball::ConsoleUI::draw() {
	DrawRectangleRounded(mainPanel, 0.05, 2, mainPanelColor);
}

void Meatball::ConsoleUI::update() {
}

void Meatball::ConsoleUI::onFocusGain() {
	Console::print("Focus gained\n");
}

void Meatball::ConsoleUI::onFocusLoss() {
	Console::print("Focus lost\n");
}