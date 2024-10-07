#include "ConsoleUI.h"

#include "Console.h"
#include "Utils/DrawFuncs.h"
#include "Utils/Utils.h"
#include "MouseCursor.h"
#include "EventHandler.h"

static void handleInputHistoryPos(int key, Meatball::InputTextBox& inputBox, std::string* inputHistory, unsigned char inputHistorySize, unsigned char inputHistoryPos) {
    if (key == KEY_UP && inputHistoryPos != 0)
        --inputHistoryPos;
    
    else if (key == KEY_DOWN && inputHistoryPos != inputHistorySize-1)
        ++inputHistoryPos;
    
    else return;
    
    if (inputBox.setText(inputHistory[inputHistoryPos])) {
		inputBox.cursorPos = inputHistory[inputHistoryPos].size();
		inputBox.onTextChange(inputBox.getText());
	}
}

Meatball::ConsoleUI::ConsoleUI(const Rectangle& rect, bool visible)
 : visible(visible), mainPanel(rect) {
	mainPanel.onMove = [this]() {
		this->closeButton.rect.x = this->mainPanel.rect.x+this->mainPanel.rect.width-this->closeButton.rect.width;
		this->closeButton.rect.y = this->mainPanel.rect.y+this->closeButton.rect.height;

		this->outputBox.setPosition(
			this->mainPanel.rect.x+this->margin,
			this->mainPanel.rect.y+this->labelFont.baseSize+this->margin);

		this->inputBox.rect.x = this->mainPanel.rect.x+this->margin;
		this->inputBox.rect.y = this->mainPanel.rect.y+this->mainPanel.rect.height-this->margin-this->inputBox.rect.height;
	};

	inputBox.rect.height = mainPanel.rect.height*0.04;

	mainPanel.onResize = [this]() {
			this->closeButton.rect.width = this->margin; // it's inside the margin
			this->closeButton.rect.height = this->margin;

			this->inputBox.rect.width = this->mainPanel.rect.width-margin*2;
			this->outputBox.fontSize = this->inputBox.fontSize = (int)this->inputBox.rect.height - 2 + (int)this->inputBox.rect.height % 2;

			this->outputBox.setSize(this->inputBox.rect.width, this->mainPanel.rect.height-this->labelFont.baseSize-1-this->margin*2-this->inputBox.rect.height);

			this->mainPanel.onMove();
	};

	mainPanel.onResizeStop = [this]() {
		this->outputBox.updateTextWrap(this->outputFont);
	};

	onResize(1, 1);

	// add auto completion
	inputBox.onTextChange = [this](const std::string& text) {
		this->autoCompleteText.clear();

		if (this->inputBoxOriginalText != text) {
			this->autoCompleteSelectedIdxBegin = 0;
			this->autoCompleteSelectedIdxEnd = 0;
			this->inputBoxOriginalText = text;
		}
		
		size_t textSize = text.size();
		if (textSize == 0) {
			return;
		}

		size_t spaceIdx = text.find(" ");
		std::string commandName = text.substr(0, spaceIdx);
		SweatCI::Command command;
		if (SweatCI::Command::getCommand(commandName, command, false)) {
			this->autoCompleteText.emplace_back(command.name+" "+command.usage, this->autoCompleteTextColor);
			return;
		}

		for (auto &command : SweatCI::Command::getCommands()) {
			size_t idx = command.name.find(text);
			if (idx == std::string::npos) continue;

			std::string leftText = command.name.substr(0, idx);
			if (leftText.size() != 0)
				this->autoCompleteText.emplace_back(leftText, this->autoCompleteTextColor);
			
			this->autoCompleteText.emplace_back(command.name.substr(idx, textSize), this->autoCompleteHighlightedTextColor); // middleText
			
			this->autoCompleteText.emplace_back(command.name.substr(idx+textSize)+" ", this->autoCompleteTextColor); // rightText
		}
	};

	inputBox.onSend = [this](const std::string& text) {
		this->autoCompleteText.clear();
		this->autoCompleteSelectedIdxBegin = this->autoCompleteSelectedIdxEnd = 0;
		this->print(SweatCI::OutputLevel::DEFAULT, text);
		Console::run(text);
		this->addToInputHistory(text);
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

	closeButton.onMouseRelease = [this]() {
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

void Meatball::ConsoleUI::draw() const {
	if (!visible) return;

	drawRect(mainPanel.rect, backgroundColor);
	
	DrawRectangle(inputBox.rect.x, inputBox.rect.y, inputBox.rect.width, inputBox.rect.height, inputBoxTheme.color);
	inputBox.draw(inputFont, inputBoxTheme.textColor, inputBoxTheme.cursorColor, inputBoxTheme.selectionColor);

	outputBox.draw(outputBoxTheme.color, outputFont);
	outputBox.scrollBar.draw(
		outputScrollBarTheme.barColor,
		outputScrollBarTheme.barHoveredColor,
		outputScrollBarTheme.thumbColor,
		outputScrollBarTheme.thumbHoveredColor1,
		outputScrollBarTheme.thumbHoveredColor2);

	if (autoCompleteText.size() != 0) {
		// I can't use draw method since I'm using offsetX
		float offsetX = 0;
		if (autoCompleteSelectedIdxEnd != 0) {
			float selectedTextWidth = 0;
			bool passedThroughSelectedText = false;
			
			for (auto &coloredText : autoCompleteText) {
				if (coloredText.color == autoCompleteSelectedTextColor) {
					selectedTextWidth += Meatball::measureTextWidth(inputFont, inputBox.fontSize, coloredText.text.c_str());
					passedThroughSelectedText = true;
					continue;
				}

				if (passedThroughSelectedText) break;
				
				offsetX += Meatball::measureTextWidth(inputFont, inputBox.fontSize, coloredText.text.c_str())+1;
			}

			if (offsetX+selectedTextWidth < mainPanel.rect.width) offsetX = 0;
			
			else offsetX -= (mainPanel.rect.width-selectedTextWidth)*0.5;
		}

		float autoCompleteY = mainPanel.rect.y+mainPanel.rect.height-margin;
		DrawRectangle(mainPanel.rect.x, autoCompleteY, mainPanel.rect.width, inputBox.rect.height, autoCompleteColor);
		BeginScissorMode(mainPanel.rect.x, autoCompleteY, mainPanel.rect.width, inputBox.rect.height);

		float x = mainPanel.rect.x;
		for (auto &coloredText : autoCompleteText) {
			drawText(inputFont, inputBox.fontSize, coloredText.text.c_str(), x-offsetX, autoCompleteY+1, coloredText.color);
			x += Meatball::measureTextWidth(inputFont, inputBox.fontSize, coloredText.text.c_str())+1;
		}

		EndScissorMode();
	}

	drawText(labelFont, labelFont.baseSize, labelText.c_str(), mainPanel.rect.x+margin, mainPanel.rect.y+margin, labelTextColor);

	drawX(closeButton.rect, closeButton.hovered? closeButtonTheme.color : closeButtonTheme.hoveredColor);
}

void Meatball::ConsoleUI::onResize(float ratioWidth, float ratioHeight) {
	mainPanel.grabHeight = labelFont.baseSize;

	mainPanel.rect.x *= ratioWidth;
    mainPanel.rect.y *= ratioHeight;

	mainPanel.rect.width *= ratioWidth;
	mainPanel.rect.height *= ratioHeight;

	inputBox.rect.height *= ratioHeight;

	mainPanel.minSize = {
		// scrollBarWidth + (margin left + margin right) + labelText size
		(int)outputBox.scrollBar.getRect().width+margin*2+Meatball::measureTextWidth(labelFont, labelFont.baseSize, labelText.c_str()),
		// outputBox minSize + inputBox minSize + (margin left + margin right)
		(int)outputBox.fontSize+inputBox.rect.height+margin*2+labelFont.baseSize};
	
	mainPanel.onResize();
	mainPanel.onResizeStop();
}

void Meatball::ConsoleUI::onCharPress(int codepoint) {
	if (!visible) return;

	inputBox.onCharPress(codepoint);
}

void Meatball::ConsoleUI::onKeyboardPress(int key) {
	if (!visible) return;
	
	inputBox.onKeyboardPress(key, inputFont);

	if (!inputBox.focused) return;
	if (key != KEY_TAB || this->autoCompleteText.size() == 0) {
		if (inputHistorySize != 0)
			handleInputHistoryPos(key, inputBox, inputHistory, inputHistorySize, inputHistoryPos);
		return;
	}

	// -1
	bool isLeftShiftDown = IsKeyDown(KEY_LEFT_SHIFT);
	if (isLeftShiftDown && autoCompleteSelectedIdxBegin != 0) {
		inputBox.onTextChange(inputBoxOriginalText);

		autoCompleteSelectedIdxEnd = autoCompleteSelectedIdxBegin-1;
		
		size_t idx = autoCompleteSelectedIdxEnd;
		autoCompleteText[idx].color = autoCompleteSelectedTextColor;
		std::string newText = autoCompleteText[idx].text;
		
		--idx;
		while (autoCompleteText[idx].text.back() != ' ') {
			autoCompleteText[idx].color = autoCompleteSelectedTextColor;
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
		autoCompleteText[idx].color = autoCompleteSelectedTextColor;
		std::string newText = autoCompleteText[idx].text;
		
		while (autoCompleteText[idx].text.back() != ' ') {
			++idx;
			autoCompleteText[idx].color = autoCompleteSelectedTextColor;
			newText += autoCompleteText[idx].text;
		}
		
		newText.pop_back();
		if (inputBox.setText(newText)) {
			inputBox.cursorPos = newText.size();
			autoCompleteSelectedIdxEnd = idx;
		}
	}
}

void Meatball::ConsoleUI::onMousePress(int button) {
	if (!visible) return;

	inputBox.onMousePress(button, inputFont);

	mainPanel.onMousePress(button);
}

void Meatball::ConsoleUI::onMouseRelease(int button) {
	if (!visible) return;

	if (button == MOUSE_BUTTON_LEFT && closeButton.hovered)
		closeButton.onMouseRelease();
	
	inputBox.onMouseRelease(button);
	
	mainPanel.onMouseRelease(button);
}

void Meatball::ConsoleUI::onMouseMove(const Vector2& mousePosition) {
	if (!visible) return;

	closeButton.onMouseMove(mousePosition);
	
	outputBox.scrollBar.onMouseMove(outputBox.getRect(), mousePosition);
	
	inputBox.onMouseMove(mousePosition);
	
	mainPanel.onMouseMove(mousePosition);
}