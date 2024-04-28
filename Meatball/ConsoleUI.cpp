#include "ConsoleUI.h"

#include "Node.h" // actually not use Node but rather use future classes: Button.h ToggleButton.h TextInput.h Panel.h etc

#include "Console.h"

Meatball::Scene* Meatball::createConsoleUI(float x, float y, float width, float height, Color mainPanelColor, bool isVisible) {
	Scene* scene = new Scene(isVisible);
	
	//Panel mainPanel{0,0};

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
	
	return scene;
}