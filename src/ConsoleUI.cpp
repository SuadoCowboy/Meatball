#include "ConsoleUI.h"

#include <HayBCMD.h>

#include "Console.h"
#include "Utils/DrawFuncs.h"
#include "FontsHandler.h"

using fh = Meatball::FontsHandler;

unsigned char Meatball::ConsoleUIScene::margin = 4;

Meatball::Config::ConsoleUI::ConsoleUI()
 : autoCompleteColor(BLACK), autoCompleteTextColor(WHITE),
 autoCompleteHighlightedTextColor(YELLOW), autoCompleteSelectedTextColor(PURPLE),
 labelTextColor(WHITE), labelText("") {
	mainFont = labelFont = fh::get("default");
}

Meatball::ConsoleUIScene::ConsoleUIScene(Rectangle rect, std::shared_ptr<Config::ConsoleUI> config, bool visible)
 : Scene(), config(config), visible(visible), mainPanel(rect) {
	inputHistoryPos = 0;
	inputHistorySize = 0;

	autoCompleteSelectedIdxBegin = 0;
	autoCompleteSelectedIdxEnd = 0;
	
	outputBox.config->font = config->mainFont;
	inputBox.config->font = config->mainFont;

	mainPanel.onMove = [&]() {
		closeButton.rect.x = mainPanel.rect.x+mainPanel.rect.width-margin-margin*0.5;
		closeButton.rect.y = mainPanel.rect.y+margin*0.5;

		outputBox.setPosition(mainPanel.rect.x+margin, mainPanel.rect.y+this->config->labelFont->baseSize+margin);

		inputBox.rect.x = mainPanel.rect.x+margin;
		inputBox.rect.y = mainPanel.rect.y+mainPanel.rect.height-margin-21;
	};
	
	mainPanel.config->grabHeight = config->labelFont->baseSize;

	mainPanel.onResize = [&]() {
			closeButton.rect.width = margin; // is inside the margin
			closeButton.rect.height = margin;

			outputBox.setSize(mainPanel.rect.width-margin*2, mainPanel.rect.height-this->config->labelFont->baseSize-1-margin*2-21);
			
			inputBox.rect.width = outputBox.getRect().width;
			inputBox.rect.height = 21;

			mainPanel.onMove();
	};

	mainPanel.onResizeStop = [&]() {
		outputBox.updateTextWrap();
	};

	mainPanel.onResize();

	mainPanel.config->minSize = {
		// scrollBarWidth + (margin left + margin right) + labelText size
		(int)outputBox.getScrollBar().getRect().width+margin*2+fh::MeasureTextWidth(config->labelFont, config->labelText),
		// outputBox minSize + inputBox minSize + (margin left + margin right)
		(int)outputBox.config->font->baseSize+inputBox.rect.height+margin*2};

	// add auto completion
	inputBox.onTextChange = [&](const char *text) {
		autoCompleteText.clear();

		if (inputBoxOriginalText != text) {
			autoCompleteSelectedIdxBegin = 0;
			autoCompleteSelectedIdxEnd = 0;
			inputBoxOriginalText = text;
		}
		
		size_t textSize = strlen(text);
		if (textSize == 0) {
			return;
		}

		size_t spaceIdx = TextFindIndex(text, " ");
		std::string commandName = TextSubtext(text, 0, spaceIdx);
		HayBCMD::Command *pCommand = HayBCMD::Command::getCommand(commandName, false);
		if (pCommand) {
			autoCompleteText.push_back({pCommand->name+" "+pCommand->usage, config->autoCompleteTextColor});
			return;
		}

		for (auto &command : HayBCMD::Command::getCommands()) {
			size_t idx = command.name.find(text);
			if (idx == std::string::npos) continue;

			std::string leftText = command.name.substr(0, idx);
			if (leftText.size() != 0)
				autoCompleteText.push_back({leftText, config->autoCompleteTextColor});
			
			autoCompleteText.push_back({command.name.substr(idx, textSize), config->autoCompleteHighlightedTextColor}); // middleText
			
			autoCompleteText.push_back({command.name.substr(idx+textSize)+" ", config->autoCompleteTextColor}); // rightText
		}
	};

	inputBox.onSend = [&](const char *text) {
		autoCompleteText.clear();
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

void Meatball::ConsoleUIScene::print(const std::string &message) {
	if (message[message.size()-1] == '\n')
		outputBox.appendText(message.substr(0,message.size()-1));
	else
		outputBox.appendText(message);
}

void Meatball::ConsoleUIScene::draw() {
	if (!visible) return;

	drawRect(mainPanel.rect, mainPanel.config->color);
	
	DrawRectangle(inputBox.rect.x, inputBox.rect.y, inputBox.rect.width, inputBox.rect.height, inputBox.config->color);
	inputBox.draw();

	outputBox.draw();
	outputBox.drawScrollbar();

	if (autoCompleteText.size() != 0) {
		// I can't use draw method since I'm using offsetX
		float offsetX = 0;
		if (autoCompleteSelectedIdxEnd != 0) {
			float selectedTextWidth = 0;
			bool passedThroughSelectedText = false;
			
			for (auto &pair : autoCompleteText) {
				if (pair.second == config->autoCompleteSelectedTextColor) {
					selectedTextWidth += fh::MeasureTextWidth(config->mainFont, pair.first.c_str());
					passedThroughSelectedText = true;
					continue;
				}

				if (passedThroughSelectedText) break;
				
				offsetX += fh::MeasureTextWidth(config->mainFont, pair.first.c_str())+1;
			}

			if (offsetX+selectedTextWidth < config->mainFont->baseSize) offsetX = 0;
			
			else offsetX -= (mainPanel.rect.width-selectedTextWidth)*0.5;
		}

		float autoCompleteY = mainPanel.rect.y+mainPanel.rect.height-margin;
		DrawRectangle(mainPanel.rect.x, autoCompleteY, mainPanel.rect.width, config->mainFont->baseSize, config->autoCompleteColor);
		BeginScissorMode(mainPanel.rect.x, autoCompleteY, mainPanel.rect.width,config->mainFont->baseSize);

		float x = mainPanel.rect.x;
		for (auto &pair : autoCompleteText) {
			drawText(config->mainFont, pair.first.c_str(), x-offsetX, autoCompleteY+1, pair.second);
			x += fh::MeasureTextWidth(config->mainFont, pair.first.c_str())+1;
		}

		EndScissorMode();
	}

	drawText(config->labelFont, config->labelText, mainPanel.rect.x+margin, mainPanel.rect.y+margin, config->labelTextColor);

	drawX(closeButton.rect, closeButton.isHovered()? closeButton.config->hoveredColor : closeButton.config->color);
}