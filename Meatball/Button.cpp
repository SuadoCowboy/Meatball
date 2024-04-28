#include "Button.h"

#include "Console.h"

Meatball::Interface::ButtonStyleClass::~ButtonStyleClass() {}

void Meatball::Interface::ButtonStyleRectangle::draw(const Button& button) {
	DrawRectangle(button.x, button.y, button.width, button.height, button.color);
}

bool Meatball::Interface::ButtonStyleRectangle::checkCollision(const Button& button, Rectangle& rectangle) {
	Rectangle buttonRect{ (float)button.x, (float)button.y, (float)button.width, (float)button.height };
	return CheckCollisionRecs(buttonRect, rectangle);
}

void Meatball::Interface::ButtonStyleCircle::draw(const Button& button) {
	DrawCircle(button.x, button.y, ((float)button.width) / 100, button.color);
}

bool Meatball::Interface::ButtonStyleCircle::checkCollision(const Button& button, Rectangle& rectangle) {
	Vector2 buttonCenter = { (float)button.x, (float)button.y };
	return CheckCollisionCircleRec(buttonCenter, ((float)button.width) / 100, rectangle);
}

void Meatball::Interface::ButtonStyleX::draw(const Button& button) {
	/*
	v1 ------- v2
	|          |
	|          |
	|          |
	v3 ------- v4

	v1 = (0,0)
	v2 = (1,0)
	v3 = (0,1)
	v4 = (1,1)

	x, y = (0,0)
	line1 = v1 -> v4
	line2 = v3 -> v2

	v1 = (x,y)
	v2 = (x+width,y)
	v3 = (x,y+height)
	v4 = (x+width,y+height)
	*/

	DrawLine(button.x, button.y, button.x + button.width, button.y + button.height, button.color); // v1 -> v4
	DrawLine(button.x, button.y + button.height, button.x + button.width, button.y, button.color); // v2 -> v3
}

bool Meatball::Interface::ButtonStyleX::checkCollision(const Button& button, Rectangle& rectangle) {
	Rectangle buttonRect{ (float)button.x, (float)button.y, (float)button.width, (float)button.height };
	return CheckCollisionRecs(buttonRect, rectangle);
}


Meatball::Interface::Button::Button(int x, int y, int width, int height, Color color, bool visible) :
	NodeUI(x, y, width, height, visible), color(color), styleMethods(nullptr) {
	setStyle(ButtonStyle::RECTANGLE);
}

Meatball::Interface::Button::~Button() {
	if (styleMethods != nullptr) {
		delete styleMethods;
		styleMethods = nullptr;
	}
}

void Meatball::Interface::Button::setStyle(ButtonStyle style) {
	if (styleMethods != nullptr) {
		delete styleMethods;
		styleMethods = nullptr;
	}
	
	if (style == ButtonStyle::RECTANGLE)
		styleMethods = new ButtonStyleRectangle();

	else if (style == ButtonStyle::CIRCLE)
		styleMethods = new ButtonStyleCircle();
	
	else if (style == ButtonStyle::X)
		styleMethods = new ButtonStyleX();
	
	else {
		styleMethods = new ButtonStyleRectangle();
		Console::print("WARNING: button created without a recognized style. Using rectangle style\n");
	}
}

void Meatball::Interface::Button::draw() {
	styleMethods->draw(*this);
}

bool Meatball::Interface::Button::checkCollision(Rectangle& rectangle) {
	return styleMethods->checkCollision(*this, rectangle);
}

void Meatball::Interface::Button::setOnMouseButtonPressed(const std::function<void(Button&, MouseButton)>& lambda) {
	onMouseButtonPressedFunction = lambda;
}

void Meatball::Interface::Button::onMouseButtonPressed(MouseButton button) {
	onMouseButtonPressedFunction(*this, button);
}

int Meatball::Interface::Button::getTypes() {
	return NodeUI::getTypes() | NodeType::NODEUI_BUTTON;
}