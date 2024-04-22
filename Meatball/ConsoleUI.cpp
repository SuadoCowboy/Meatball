#include "ConsoleUI.h"

#include "Console.h"

Meatball::ConsoleUI::ConsoleUI() {}
Meatball::ConsoleUI::~ConsoleUI() {}

void Meatball::ConsoleUI::handleInput() {}
void Meatball::ConsoleUI::draw() {
	
}

void Meatball::ConsoleUI::update() {}

void Meatball::ConsoleUI::onFocusGain() {
	Console::print("Focus gained\n");
}

void Meatball::ConsoleUI::onFocusLoss() {
	Console::print("Focus lost\n");
}