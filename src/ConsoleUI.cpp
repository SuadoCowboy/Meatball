#include "ConsoleUI.h"

#include <HayBCMD.h>

#include "Console.h"

unsigned char Meatball::ConsoleUIScene::margin = 4;

Meatball::ConsoleUIScene::ConsoleUIScene(float x, float y, float width, float height, Font* font, bool visible)
	: Scene(), visible(visible) {
	mainPanel = {x, y, width, height};
	
	inputHistoryPos = 0;
	inputHistorySize = 0;

	autoCompleteTextColor = WHITE;
	autoCompleteHighlightedTextColor = YELLOW;
	autoCompleteSelectedTextColor = PURPLE;

	autoCompleteSelectedIdxBegin = 0;
	autoCompleteSelectedIdxEnd = 0;
	
	outputBox.font = font;
	inputBox.font = font;
	autoCompleteBox.font = font;

	mainPanel.onMove = [&]() {
		closeButton.rect.x = mainPanel.rect.x+mainPanel.rect.width-margin-margin*0.5;
		closeButton.rect.y = mainPanel.rect.y+margin*0.5;

		outputBox.setPosition(mainPanel.rect.x+margin, mainPanel.rect.y+margin);

		inputBox.rect.x = mainPanel.rect.x+margin;
		inputBox.rect.y = mainPanel.rect.y+mainPanel.rect.height-margin-21;

		autoCompleteBox.rect.x = mainPanel.rect.x+margin;
		autoCompleteBox.rect.y = mainPanel.rect.y+mainPanel.rect.height-margin-42;
	};
	
	mainPanel.onResize = [&]() {
			// TODO: FIXME: console is glitchy af

			closeButton.rect.width = (float)margin; // is inside the margin
			closeButton.rect.height = (float)margin;

			inputBox.rect.width = mainPanel.rect.width-margin*2-3;
			inputBox.rect.height = 21;

			autoCompleteBox.rect.width = mainPanel.rect.width-margin*2-3;
			autoCompleteBox.rect.height = 21;

			outputBox.setSize(mainPanel.rect.width-margin*2-3, mainPanel.rect.height-margin*2-42);

			mainPanel.onMove();
	};

	mainPanel.onResizeStop = [&]() {
		outputBox.updateTextWrap();
	};

	mainPanel.onResize();

	// add auto completion
	inputBox.onTextChange = [&](const std::string& text) {
		autoCompleteBox.coloredText.clear();

		if (inputBoxOriginalText != text) {
			autoCompleteSelectedIdxBegin = 0;
			autoCompleteSelectedIdxEnd = 0;
			inputBoxOriginalText = text;
		}
		
		if (text.size() == 0) {
			return;
		}

		size_t spaceIdx = text.find(' ');
		std::string commandName = text.substr(0, spaceIdx);
		HayBCMD::Command* pCommand = HayBCMD::Command::getCommand(commandName, false);
		if (pCommand) {
			autoCompleteBox.pushText(pCommand->name+" "+pCommand->usage, autoCompleteTextColor);
			return;
		}

		for (auto& command : HayBCMD::Command::getCommands()) {
			size_t idx = command.name.find(text);
			if (idx == std::string::npos) continue;

			std::string leftText = command.name.substr(0, idx);
			if (leftText.size() != 0)
				autoCompleteBox.pushText(leftText, autoCompleteTextColor);
			
			autoCompleteBox.pushText(command.name.substr(idx, text.size()), autoCompleteHighlightedTextColor); // middleText
			
			autoCompleteBox.pushText(command.name.substr(idx+text.size())+" ", autoCompleteTextColor); // rightText
		}
	};

	inputBox.onSend = [&](const std::string& text) {
		autoCompleteBox.coloredText.clear();
		print(text);
		Console::run(text);
		addToInputHistory(text);
	};

	/*
	---------------------
	{Console           x}
	{  {output      }   }
	{  {            }   }
	{  {            }   }
	{  {            }   }
	{                   }
	{{auto complete    }}
	{{input text       }}
	---------------------
	*/

	closeButton.onRelease = [&]() {
		this->visible = false; // WHY USING this->visible WORKS AND visible = false DOESN'T?
		// TODO: fading effect (maybe add a task system? Meatball::Task() or Meatball::newTask() or something)
	};

	/*
	TODOS:
	rounded border in mainPanel
	a text on the top left written: Console or Local Console(developers can change this how they want)
	F1 key as default to toggle the local console
	buildVersion on the top right OF THE SCREEN, NOT ON THE PANEL
	*/
}

void Meatball::ConsoleUIScene::print(const std::string& message) {
	// if you want a newline you will need to put "\n\n"
	if (message[message.size()-1] == '\n')
		outputBox.appendText(message.substr(0,message.size()-1));
	else
		outputBox.appendText(message);
}

void Meatball::ConsoleUIScene::draw() {
	if (!visible) return;
	mainPanel.draw();
	
	autoCompleteBox.draw();

	inputBox.draw();
	outputBox.draw();

	closeButton.drawX();
}