#include "Button.h"

#include "Console.h"

Meatball::Interface::ButtonDraw::~ButtonDraw() {}

class ButtonDrawRectangle : public Meatball::Interface::ButtonDraw {
public:
	void draw(const Meatball::Interface::Button& button) override {
		DrawRectangle(button.x, button.y, button.width, button.height, button.color);
	}
};

class ButtonDrawCircle : public Meatball::Interface::ButtonDraw {
public:
	void draw(const Meatball::Interface::Button& button) override {
		DrawCircle(button.x, button.y, ((float)button.width) / 100, button.color);
	}
};

class ButtonDrawX : public Meatball::Interface::ButtonDraw {
public:
	void draw(const Meatball::Interface::Button& button) override {
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
};

Meatball::Interface::Button::Button(int x, int y, int width, int height, Color color, bool visible) :
	NodeUI(x, y, width, height, visible), color(color), drawMethod(nullptr) {
	setStyle(ButtonStyle::RECTANGLE);
}

Meatball::Interface::Button::~Button() {
	if (drawMethod != nullptr) {
		delete drawMethod;
		drawMethod = nullptr;
	}
}

void Meatball::Interface::Button::setStyle(ButtonStyle style) {
	if (drawMethod != nullptr) {
		delete drawMethod;
		drawMethod = nullptr;
	}
	
	switch (style) {
	case ButtonStyle::RECTANGLE:
		drawMethod = new ButtonDrawRectangle();
		break;

	case ButtonStyle::CIRCLE:
		drawMethod = new ButtonDrawCircle();
		break;
	
	case ButtonStyle::X:
		drawMethod = new ButtonDrawX();
		break;
	default:
		Console::print("WARNING: button created without a recognized style. If draw is called it might harm something");
		break;
	}
}

void Meatball::Interface::Button::draw() {
	drawMethod->draw(*this);
}

void Meatball::Interface::Button::onMouseButtonPressed(Input::MOUSEBUTTONS) {
	//if (Input::mouseButtonPressedOnce(MouseButton::MOUSE_BUTTON_LEFT))
	//	do something
}

Meatball::NodeType Meatball::Interface::Button::getType() {
	return NodeType::NODEUI_BUTTON;
}