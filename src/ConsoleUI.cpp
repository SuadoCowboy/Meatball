#include "ConsoleUI.h"

#include <HayBCMD.h>

#include "Console.h"

#include <iostream>

unsigned char Meatball::ConsoleUIScene::margin = 4;

Meatball::ConsoleUIScene::ConsoleUIScene(float x, float y, float width, float height, bool visible)
	: Scene(), visible(visible) {
	sendButton = {x+width-76-margin, y+height-21-margin, 76, 21};
	mainPanel = {x, y, width, height};
	closeButton = {x+width-5-margin, y+margin, 5, 5};

	sendButton.setText("", sendButton.rect.height-4);

	closeButton.onRelease = [&]() {
		visible = false;
		// TODO: fading effect (maybe add a task system? Meatball::Task() or Meatball::newTask() or something)
	};

	sendButton.onRelease = [&]() {
		//print(inputTextbox.getText());
		//Console::run(inputTextbox.getText());
		//inputTextbox.setText("");
	};

	/*Panel* mainPanel = new Panel{ x, y, width, height, mainPanelColor };
	mainPanel->z = 1;
	
	Button* closeButton = new Button{ x + width - 10.f, y + 5.f, 5.f, 5.f, {255, 255, 255, 255} };
	closeButton->setStyle(ButtonStyle::X);
	closeButton->setAnchor(mainPanel);
	closeButton->z = 0;

	HayBCMD::Output::print("(" + std::to_string(closeButton->x) + ", " + std::to_string(closeButton->y) + ")\n");

	closeButton->connectOnMouseButtonPressed([&](Scene& scene, Button& button, Input::InpMouseButton mouseButton)  {
		// TODO: FUTURE: slow fade when closing(configurable via cmd so settings.cfg could be created :D)
		if (mouseButton & Input::InpMouseButton::MOUSE1)
			scene.visible = false;
	});

	scene->addNode(mainPanel);
	scene->addNode(closeButton);

	// this should be called after adding all nodes
	scene->sortUINodesVector();

	//inputBox.setAnchor(mainPanel);
	//outputBox.setAnchor(mainPanel);
	// !!!buildVersion does not require a anchor as it is static in the screen!!!
	//mainPanel.position = {middleScreenW, middleScreenH};
	//closeButton.connectOnClick([&]() -> { mainPanel.visible = false; });
	//inputBox.connectOnSend([&]() -> { Console::run(inputBox.text); outputBox.text = Console::output /*maybe text is ptr?});
	
	//scene->addNode(mainPanel); // with a pretty rounded border and stuff
	//scene->addNode(closeButton); // the button at the top right to close the console if the user doesn't want to close with the keyboard
	//scene->addNode(inputBox) // the inputBox filling the down part without counting the borders
	//scene->addNode(outputBox); // the box that shows all the console data
	//scene->addNode(buildVersion) // this one is at the top right of the window itself
	*/
}

void Meatball::ConsoleUIScene::print(const std::string& message) {
	//outputTextbox.appendText(message);
	std::cout << message; // THIS IS TEMPORARY!!! Todo: remove this after creating outputTextbox and make it work
}

void Meatball::ConsoleUIScene::draw() {
	if (!visible) return;
	mainPanel.draw();
	
	sendButton.drawRect();
	sendButton.drawTextCentered(true, true);
	
	closeButton.drawX();
}

void Meatball::ConsoleUIScene::update() {
	if (!visible) return;

	sendButton.update();
	closeButton.update();
}