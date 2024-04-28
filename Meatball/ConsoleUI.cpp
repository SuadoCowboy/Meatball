#include "ConsoleUI.h"

#include "Panel.h"
#include "Button.h"

#include "Console.h"

using namespace Meatball::Interface;

Meatball::Scene* Meatball::createConsoleUI(int x, int y, int width, int height, Color mainPanelColor, bool visible) {
	Scene* scene = new Scene(x, y, width, height, visible);
	
	Panel* mainPanel = new Panel{ 0, 0, width, height, mainPanelColor };
	mainPanel->z = 1;

	Button* closeButton = new Button{ width - 10, 5, 5, 5, {255, 255, 255, 255} };
	closeButton->setStyle(ButtonStyle::X);
	closeButton->setAnchor(mainPanel);
	closeButton->z = 0;

	closeButton->connectOnFocusGain([&](Scene& scene, NodeUI uiNode) {
		Console::print("FOCUS GAINED BY BUTTON!\n");
	});

	closeButton->connectOnFocusLoss([&](Scene& scene, NodeUI uiNode) {
		Console::print("FOCUS LOST BY BUTTON!\n");
	});

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
	//inputBox.connectOnSend([&]() -> { Console::run(inputBox.text); outputBox.text = Console::output /*maybe text is ptr?*/});
	
	//scene->addNode(mainPanel); // with a pretty rounded border and stuff
	//scene->addNode(closeButton); // the button at the top right to close the console if the user doesn't want to close with the keyboard
	//scene->addNode(inputBox) // the inputBox filling the down part without counting the borders
	//scene->addNode(outputBox); // the box that shows all the console data
	//scene->addNode(buildVersion) // this one is at the top right of the window itself
	
	return scene;
}