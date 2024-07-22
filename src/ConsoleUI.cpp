#include "ConsoleUI.h"

#include "Console.h"
#include "Utils/DrawFuncs.h"
#include "Utils/Utils.h"
#include "MouseCursor.h"

unsigned char Meatball::ConsoleUIScene::margin = 4;

static void handleInputHistoryPos(Meatball::InputTextBox &inputBox, std::string *inputHistory, const unsigned char &inputHistorySize, unsigned char &inputHistoryPos) {
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

Meatball::Config::ConsoleUI::ConsoleUI()
 : labelFont(nullptr), autoCompleteColor(BLACK), autoCompleteTextColor(WHITE),
 autoCompleteHighlightedTextColor(YELLOW), autoCompleteSelectedTextColor(PURPLE),
 labelTextColor(WHITE) {}

Meatball::ConsoleUIScene::ConsoleUIScene(const Rectangle &rect, const std::shared_ptr<Config::ConsoleUI> &_config, bool visible)
 : Scene(), config(_config), visible(visible), mainPanel(rect) {
	// Because console interface SHOULD always exist, it's a good idea to get the default
	// config from here. any other interface that the default config is not defined in the
	// console interface should be handled by YOUR program. You could just make a config
	// pointer outside any object so whenever a object that uses that config is created,
	// you get that outside config.
	mainPanel.config = std::make_shared<Config::DynamicPanel>(Defaults::dynamicPanelConfig);
	outputBox.config = std::make_shared<Config::ScrollTextBox>(Defaults::scrollTextBoxConfig);
    inputBox.config = std::make_shared<Config::InputTextBox>(Defaults::inputTextBoxConfig);
	closeButton.config = std::make_shared<Config::Button>(Defaults::buttonConfig);

	mainPanel.onMove = [&]() {
		closeButton.rect.x = mainPanel.rect.x+mainPanel.rect.width-margin-margin*0.5;
		closeButton.rect.y = mainPanel.rect.y+margin*0.5;

		outputBox.setPosition(mainPanel.rect.x+margin, mainPanel.rect.y+config->labelFont->baseSize+margin);

		inputBox.rect.x = mainPanel.rect.x+margin;
		inputBox.rect.y = mainPanel.rect.y+mainPanel.rect.height-margin-inputBox.rect.height;
	};

	inputBox.rect.height = mainPanel.rect.height*0.04;

	mainPanel.onResize = [&]() {
			closeButton.rect.width = margin; // it's inside the margin
			closeButton.rect.height = margin;

			inputBox.rect.width = mainPanel.rect.width-margin*2;
			outputBox.config->fontSize = inputBox.config->fontSize = (int)inputBox.rect.height - 2 + (int)inputBox.rect.height % 2;

			outputBox.setSize(inputBox.rect.width, mainPanel.rect.height-config->labelFont->baseSize-1-margin*2-inputBox.rect.height);

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

	inputBox.onSend = [&](const std::string& text) {
		autoCompleteText.clear();
		autoCompleteSelectedIdxBegin = autoCompleteSelectedIdxEnd = 0;
		print(HayBCMD::OutputLevel::DEFAULT, text);
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

void Meatball::ConsoleUIScene::update() {
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
				autoCompleteText[idx].second = config->autoCompleteSelectedTextColor;
				std::string newText = autoCompleteText[idx].first;
				
				--idx;
				while (autoCompleteText[idx].first.back() != ' ') {
					autoCompleteText[idx].second = config->autoCompleteSelectedTextColor;
					newText = autoCompleteText[idx].first+newText;
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
				autoCompleteText[idx].second = config->autoCompleteSelectedTextColor;
				std::string newText = autoCompleteText[idx].first;
				
				while (autoCompleteText[idx].first.back() != ' ') {
					++idx;
					autoCompleteText[idx].second = config->autoCompleteSelectedTextColor;
					newText += autoCompleteText[idx].first;
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
					selectedTextWidth += Meatball::measureTextWidth(*inputBox.config->font, inputBox.config->fontSize, pair.first.c_str());
					passedThroughSelectedText = true;
					continue;
				}

				if (passedThroughSelectedText) break;
				
				offsetX += Meatball::measureTextWidth(*inputBox.config->font, inputBox.config->fontSize, pair.first.c_str())+1;
			}

			if (offsetX+selectedTextWidth < mainPanel.rect.width) offsetX = 0;
			
			else offsetX -= (mainPanel.rect.width-selectedTextWidth)*0.5;
		}

		float autoCompleteY = mainPanel.rect.y+mainPanel.rect.height-margin;
		DrawRectangle(mainPanel.rect.x, autoCompleteY, mainPanel.rect.width, inputBox.rect.height, config->autoCompleteColor);
		BeginScissorMode(mainPanel.rect.x, autoCompleteY, mainPanel.rect.width, inputBox.rect.height);

		float x = mainPanel.rect.x;
		for (auto &pair : autoCompleteText) {
			drawText(*inputBox.config->font, inputBox.config->fontSize, pair.first.c_str(), x-offsetX, autoCompleteY+1, pair.second);
			x += Meatball::measureTextWidth(*inputBox.config->font, inputBox.config->fontSize, pair.first.c_str())+1;
		}

		EndScissorMode();
	}

	drawText(*config->labelFont, config->labelFont->baseSize, config->labelText, mainPanel.rect.x+margin, mainPanel.rect.y+margin, config->labelTextColor);

	drawX(closeButton.rect, closeButton.isHovered()? closeButton.config->hoveredColor : closeButton.config->color);
}

void Meatball::ConsoleUIScene::onResize(float ratioWidth, float ratioHeight) {
	mainPanel.config->grabHeight = config->labelFont->baseSize;

	mainPanel.rect.x *= ratioWidth;
    mainPanel.rect.y *= ratioHeight;

	mainPanel.rect.width *= ratioWidth;
	mainPanel.rect.height *= ratioHeight;

	inputBox.rect.height *= ratioHeight;

	mainPanel.config->minSize = {
		// scrollBarWidth + (margin left + margin right) + labelText size
		(int)outputBox.getScrollBar().getRect().width+margin*2+Meatball::measureTextWidth(*config->labelFont, config->labelFont->baseSize, config->labelText),
		// outputBox minSize + inputBox minSize + (margin left + margin right)
		(int)outputBox.config->font->baseSize+inputBox.rect.height+margin*2+config->labelFont->baseSize};
	
	mainPanel.onResize();
	mainPanel.onResizeStop();
}