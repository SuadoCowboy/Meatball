#include "ConsoleUI.h"

#include "Node.h" // actually not use Node but rather use future classes: Button.h ToggleButton.h TextInput.h Panel.h etc

#include "Console.h"

Meatball::Scene* Meatball::createConsoleUI(float x, float y, float width, float height, Color mainPanelColor, bool isVisible) {
	Rectangle mainPanel; // might need to be created a node to store the mainPanel also...

	//Scene* scene = new Scene(isVisible);
	/*
	delete scene;
	scene = nullptr; // doing that in case this part is ran WHILE IT'S NOT EVEN FINISHED
	*/
	
	//closeButton.setAnchor(mainPanel);
	//inputBox.setAnchor(mainPanel);
	//outputBox.setAnchor(mainPanel);
	// !!!buildVersion does not require a anchor as it is static in the screen!!!
	//mainPanel.position = {middleScreenW, middleScreenH};
	//closeButton.connectOnClick([&]() -> { mainPanel.visible = false; });
	//inputBox.connectOnSend([&]() -> { Console::run(inputBox.text); outputBox.text = Console::output /*maybe text is ptr?*/});
	
	//scene->addNode(mainPanel); // with a pretty rounded border and stuff
	//scene->addNode(closeButton); // the button at the top right to close the console if the user doesn't want to close with the keyboard
	//scene->addNode(inputBox) // the inputBox filling the down part without counting the borders
	//scene->addNode(outputBox); // the box that shows all the console data
	//scene->addNode(buildVersion) // this one is at the top right
	
	//return scene;
}