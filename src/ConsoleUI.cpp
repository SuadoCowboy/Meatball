#include "ConsoleUI.h"

#include <HayBCMD.h>

#include "Console.h"

unsigned char Meatball::ConsoleUIScene::margin = 4;

Meatball::ConsoleUIScene::ConsoleUIScene(float x, float y, float width, float height, unsigned char fontSize, bool visible)
	: Scene(), visible(visible) {
	mainPanel = {x, y, width, height};
	
	closeButton = {x+width-margin-margin/2, y+margin/2, (float)margin, (float)margin}; // is inside the margin

	outputBox = {x+margin, y+margin, width-margin*2-3, height-margin*2-21, fontSize};
	inputBox = {x+margin, y+height-margin-21, width-margin*2-3, 21, fontSize};

	/*
	---------------------
	{Console           x}
	{  {output      }   }
	{  {            }   }
	{  {            }   }
	{  {            }   }
	{                   }
	{{input text      } }
	---------------------
	*/

	closeButton.onRelease = [&]() {
		visible = false;
		// TODO: fading effect (maybe add a task system? Meatball::Task() or Meatball::newTask() or something)
	};

	//inputBox.onSend = [&]() {
		//print(inputTextbox.getText());
		//Console::run(inputTextbox.getText());
		//inputTextbox.setText("");
	//};

	/*
	TODOS:
	rounded border in mainPanel
	a text on the top left written: Console or Local Console(developers can change this how they want)
	F1 key as default to toggle the console
	buildVersion on the top right OF THE SCREEN, NOT ON THE PANEL

	//scene->addNode(inputBox) // the inputBox filling the down part without counting the borders
	//scene->addNode(outputBox); // the box that shows all the console data
	//scene->addNode(buildVersion) // this one is at the top right of the window itself
	*/
}

void Meatball::ConsoleUIScene::print(const std::string& message) {
	outputBox.appendText(message);
}

void Meatball::ConsoleUIScene::draw() {
	if (!visible) return;
	mainPanel.draw();
	
	inputBox.draw();
	outputBox.draw();

	closeButton.drawX();
}