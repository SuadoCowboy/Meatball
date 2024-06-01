#include "ConsoleUI.h"

#include <cstring>

#include <HayBCMD.h>

#include "Console.h"
#include "FontsHandler.h"

unsigned char Meatball::ConsoleUIScene::margin = 4;

using fh = Meatball::FontsHandler;

Meatball::ConsoleUIScene::ConsoleUIScene(float x, float y, float width, float height, Font *font, Font *_labelFont, bool visible)
	: Scene(), visible(visible) {
	mainPanel = {x, y, width, height};
	
	std::strcpy(labelText, "Local Console\0");
	labelColor = WHITE;
	labelFont = _labelFont;

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

		outputBox.setPosition(mainPanel.rect.x+margin, mainPanel.rect.y+labelFont->baseSize+margin);

		inputBox.rect.x = mainPanel.rect.x+margin;
		inputBox.rect.y = mainPanel.rect.y+mainPanel.rect.height-margin-21;

		autoCompleteBox.rect.x = mainPanel.rect.x;
		autoCompleteBox.rect.y = mainPanel.rect.y+mainPanel.rect.height-margin;
	};
	
	mainPanel.grabHeight = labelFont->baseSize;

	mainPanel.onResize = [&]() {
			closeButton.rect.width = margin; // is inside the margin
			closeButton.rect.height = margin;

			autoCompleteBox.rect.width = mainPanel.rect.width;
			autoCompleteBox.rect.height = 21;

			outputBox.setSize(mainPanel.rect.width-margin*2, mainPanel.rect.height-labelFont->baseSize-1-margin*2-21);
			
			inputBox.rect.width = outputBox.getRect().width;
			inputBox.rect.height = 21;

			mainPanel.onMove();
	};

	mainPanel.onResizeStop = [&]() {
		outputBox.updateTextWrap();
	};

	mainPanel.onResize();

	mainPanel.minSize = {
		(int)outputBox.getScrollBar().getRect().width+margin*2+fh::MeasureTextWidth(labelFont, labelText),
		(int)outputBox.font->baseSize+inputBox.rect.height+margin*2};

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
		HayBCMD::Command *pCommand = HayBCMD::Command::getCommand(commandName, false);
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
	{{output           }}
	{{                 }}
	{{                 }}
	{{                 }}
	{                   }
	{{input text       }}
	{auto complete      }
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
	
	inputBox.draw();
	outputBox.draw();

	if (autoCompleteBox.coloredText.size() != 0) {
		// I can't use draw method since I'm using offsetX
		float offsetX = 0;
		if (autoCompleteSelectedIdxEnd != 0) {
			float selectedTextWidth = 0;
			bool passedThroughSelectedText = false;
			
			for (auto& pair : autoCompleteBox.coloredText) {
				if (pair.second == autoCompleteSelectedTextColor) {
					selectedTextWidth += fh::MeasureTextWidth(autoCompleteBox.font, pair.first.c_str());
					passedThroughSelectedText = true;
					continue;
				}

				if (passedThroughSelectedText) break;
				
				offsetX += fh::MeasureTextWidth(autoCompleteBox.font, pair.first.c_str())+1;
			}

			if (offsetX+selectedTextWidth < autoCompleteBox.rect.width) offsetX = 0;
			
			else offsetX -= (autoCompleteBox.rect.width-selectedTextWidth)*0.5;
		}

		DrawRectangle(autoCompleteBox.rect.x, autoCompleteBox.rect.y, autoCompleteBox.rect.width, autoCompleteBox.rect.height, autoCompleteBox.color);
		BeginScissorMode(autoCompleteBox.rect.x, autoCompleteBox.rect.y, autoCompleteBox.rect.width, autoCompleteBox.rect.height);

		float x = autoCompleteBox.rect.x;
		for (auto& pair : autoCompleteBox.coloredText) {
			fh::DrawText(autoCompleteBox.font, pair.first.c_str(), x-offsetX, autoCompleteBox.rect.y+1, pair.second);
			x += fh::MeasureTextWidth(autoCompleteBox.font, pair.first.c_str())+1;
		}

		EndScissorMode();
	}

	fh::DrawText(labelFont, labelText, mainPanel.rect.x+margin, mainPanel.rect.y+margin, labelColor);

	closeButton.drawX();
}