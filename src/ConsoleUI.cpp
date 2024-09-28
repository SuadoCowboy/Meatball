#include "ConsoleUI.h"

#include "Console.h"
#include "Utils/DrawFuncs.h"
#include "Utils/Utils.h"
#include "MouseCursor.h"

unsigned char Meatball::ConsoleUI::margin = 4;

static void handleInputHistoryPos(Meatball::InputTextBox &inputBox, std::string *inputHistory, unsigned char inputHistorySize, unsigned char &inputHistoryPos) {
    if ((IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_UP)) && inputHistoryPos != 0)
        --inputHistoryPos;
    
    else if ((IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN)) && inputHistoryPos != inputHistorySize-1)
        ++inputHistoryPos;
    
    else return;
    
    if (inputBox.setText(inputHistory[inputHistoryPos])) {
		inputBox.cursorPos = inputHistory[inputHistoryPos].size();
		inputBox.onTextChange(inputBox.getText());
	}
}

Meatball::ConsoleUI::ConsoleUI(const Rectangle& rect, const Config::ConsoleUI& _config, bool visible)
 : IScene(), config(_config), visible(visible), mainPanel(rect) {
	mainPanel.config = &Defaults::dynamicPanelConfig;
	outputBox.config = &Defaults::scrollTextBoxConfig;
    inputBox.config = &Defaults::inputTextBoxConfig;
	closeButton.config = &Defaults::buttonConfig;

	mainPanel.onMove = [&]() {
		closeButton.rect.x = mainPanel.rect.x+mainPanel.rect.width-margin-margin*0.5;
		closeButton.rect.y = mainPanel.rect.y+margin*0.5;

		outputBox.setPosition(mainPanel.rect.x+margin, mainPanel.rect.y+config.labelFont->baseSize+margin);

		inputBox.rect.x = mainPanel.rect.x+margin;
		inputBox.rect.y = mainPanel.rect.y+mainPanel.rect.height-margin-inputBox.rect.height;
	};

	inputBox.rect.height = mainPanel.rect.height*0.04;

	mainPanel.onResize = [&]() {
			closeButton.rect.width = margin; // it's inside the margin
			closeButton.rect.height = margin;

			inputBox.rect.width = mainPanel.rect.width-margin*2;
			outputBox.fontSize = inputBox.fontSize = (int)inputBox.rect.height - 2 + (int)inputBox.rect.height % 2;

			outputBox.setSize(inputBox.rect.width, mainPanel.rect.height-config.labelFont->baseSize-1-margin*2-inputBox.rect.height);

			mainPanel.onMove();
	};

	mainPanel.onResizeStop = [&]() {
		outputBox.updateTextWrap();
	};

	onResize(1, 1);

	// add auto completion
	inputBox.onTextChange = [&](const std::string& text) {
		autoCompleteText.clear();

		if (inputBoxOriginalText != text) {
			autoCompleteSelectedIdxBegin = 0;
			autoCompleteSelectedIdxEnd = 0;
			inputBoxOriginalText = text;
		}
		
		size_t textSize = text.size();
		if (textSize == 0) {
			return;
		}

		size_t spaceIdx = text.find(" ");
		std::string commandName = text.substr(0, spaceIdx);
		SweatCI::Command command;
		if (SweatCI::Command::getCommand(commandName, command, false)) {
			autoCompleteText.emplace_back(command.name+" "+command.usage, config.autoCompleteTextColor);
			return;
		}

		for (auto &command : SweatCI::Command::getCommands()) {
			size_t idx = command.name.find(text);
			if (idx == std::string::npos) continue;

			std::string leftText = command.name.substr(0, idx);
			if (leftText.size() != 0)
				autoCompleteText.emplace_back(leftText, config.autoCompleteTextColor);
			
			autoCompleteText.emplace_back(command.name.substr(idx, textSize), config.autoCompleteHighlightedTextColor); // middleText
			
			autoCompleteText.emplace_back(command.name.substr(idx+textSize)+" ", config.autoCompleteTextColor); // rightText
		}
	};

	inputBox.onSend = [&](const std::string& text) {
		autoCompleteText.clear();
		autoCompleteSelectedIdxBegin = autoCompleteSelectedIdxEnd = 0;
		print(SweatCI::OutputLevel::DEFAULT, text);
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
		this->visible = false;
		resetCursor(MouseCursorPriorityLevel::BUTTON);
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

void Meatball::ConsoleUI::update() {
	if (!visible) return;

	inputBox.update();
	
	if (inputBox.focused) {
		if ((IsKeyPressed(KEY_TAB) || IsKeyPressedRepeat(KEY_TAB)) && autoCompleteText.size() != 0) {
			// -1
			bool isLeftShiftDown = IsKeyDown(KEY_LEFT_SHIFT);
			if (isLeftShiftDown && autoCompleteSelectedIdxBegin != 0) {
				inputBox.onTextChange(inputBoxOriginalText);

				autoCompleteSelectedIdxEnd = autoCompleteSelectedIdxBegin-1;
				
				size_t idx = autoCompleteSelectedIdxEnd;
				autoCompleteText[idx].color = config.autoCompleteSelectedTextColor;
				std::string newText = autoCompleteText[idx].text;
				
				--idx;
				while (autoCompleteText[idx].text.back() != ' ') {
					autoCompleteText[idx].color = config.autoCompleteSelectedTextColor;
					newText = autoCompleteText[idx].text+newText;
					if (idx == 0) break;
					--idx;
				}
				
				newText.pop_back();
				if (inputBox.setText(newText)) {
					inputBox.cursorPos = newText.size();
					autoCompleteSelectedIdxBegin = idx == 0? 0 : idx+1;
				}
			}
			
			// +1
			if (!isLeftShiftDown && autoCompleteSelectedIdxEnd != autoCompleteText.size()-1) {
				inputBox.onTextChange(inputBoxOriginalText);
				
				autoCompleteSelectedIdxBegin = autoCompleteSelectedIdxEnd+1;
				if (autoCompleteSelectedIdxEnd == 0) autoCompleteSelectedIdxBegin = 0;
				
				size_t idx = autoCompleteSelectedIdxBegin;
				autoCompleteText[idx].color = config.autoCompleteSelectedTextColor;
				std::string newText = autoCompleteText[idx].text;
				
				while (autoCompleteText[idx].text.back() != ' ') {
					++idx;
					autoCompleteText[idx].color = config.autoCompleteSelectedTextColor;
					newText += autoCompleteText[idx].text;
				}
				
				newText.pop_back();
				if (inputBox.setText(newText)) {
					inputBox.cursorPos = newText.size();
					autoCompleteSelectedIdxEnd = idx;
				}
			}
		}

		else if (inputHistorySize != 0)
			handleInputHistoryPos(inputBox, inputHistory, inputHistorySize, inputHistoryPos);
	}

	outputBox.update();
	closeButton.update();
	mainPanel.update();
}

void Meatball::ConsoleUI::draw() {
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
			
			for (auto &coloredText : autoCompleteText) {
				if (coloredText.color == config.autoCompleteSelectedTextColor) {
					selectedTextWidth += Meatball::measureTextWidth(*inputBox.config->font, inputBox.fontSize, coloredText.text.c_str());
					passedThroughSelectedText = true;
					continue;
				}

				if (passedThroughSelectedText) break;
				
				offsetX += Meatball::measureTextWidth(*inputBox.config->font, inputBox.fontSize, coloredText.text.c_str())+1;
			}

			if (offsetX+selectedTextWidth < mainPanel.rect.width) offsetX = 0;
			
			else offsetX -= (mainPanel.rect.width-selectedTextWidth)*0.5;
		}

		float autoCompleteY = mainPanel.rect.y+mainPanel.rect.height-margin;
		DrawRectangle(mainPanel.rect.x, autoCompleteY, mainPanel.rect.width, inputBox.rect.height, config.autoCompleteColor);
		BeginScissorMode(mainPanel.rect.x, autoCompleteY, mainPanel.rect.width, inputBox.rect.height);

		float x = mainPanel.rect.x;
		for (auto &coloredText : autoCompleteText) {
			drawText(*inputBox.config->font, inputBox.fontSize, coloredText.text.c_str(), x-offsetX, autoCompleteY+1, coloredText.color);
			x += Meatball::measureTextWidth(*inputBox.config->font, inputBox.fontSize, coloredText.text.c_str())+1;
		}

		EndScissorMode();
	}

	drawText(*config.labelFont, config.labelFont->baseSize, config.labelText, mainPanel.rect.x+margin, mainPanel.rect.y+margin, config.labelTextColor);

	drawX(closeButton.rect, closeButton.hovered? closeButton.config->hoveredColor : closeButton.config->color);
}

void Meatball::ConsoleUI::onResize(float ratioWidth, float ratioHeight) {
	mainPanel.config->grabHeight = config.labelFont->baseSize;

	mainPanel.rect.x *= ratioWidth;
    mainPanel.rect.y *= ratioHeight;

	mainPanel.rect.width *= ratioWidth;
	mainPanel.rect.height *= ratioHeight;

	inputBox.rect.height *= ratioHeight;

	mainPanel.config->minSize = {
		// scrollBarWidth + (margin left + margin right) + labelText size
		(int)outputBox.getScrollBar().getRect().width+margin*2+Meatball::measureTextWidth(*config.labelFont, config.labelFont->baseSize, config.labelText),
		// outputBox minSize + inputBox minSize + (margin left + margin right)
		(int)outputBox.fontSize+inputBox.rect.height+margin*2+config.labelFont->baseSize};
	
	mainPanel.onResize();
	mainPanel.onResizeStop();
}