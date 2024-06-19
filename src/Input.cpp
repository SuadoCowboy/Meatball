#include "Input.h"

#include <algorithm>
#include <cctype>
#include <sstream>

#include "Console.h"

std::unordered_map<std::string, Meatball::InputData> Meatball::Input::keyState;
std::unordered_map<std::string, Meatball::InputData> Meatball::Input::mouseState;

std::string Meatball::Input::mouseWheelUpCallback;
std::string Meatball::Input::mouseWheelDownCallback;

void Meatball::Input::bind(std::string keyName, const std::string& callback) {
	std::transform(keyName.begin(), keyName.end(), keyName.begin(),
    [](unsigned char c){ return std::tolower(c); });

	bool isMouseButton = mouseState.count(keyName) != 0;
	bool isMouseWheelUp = keyName == "mwheelup";
	bool isMouseWheelDown = keyName == "mwheeldown";

	if (isMouseWheelUp)
		mouseWheelUpCallback = callback;
	else if (isMouseWheelDown)
		mouseWheelDownCallback = callback;
	else if (keyState.count(keyName) == 0 && !isMouseButton) return;
	
	if (!isMouseButton) keyState[keyName].callback = callback;
	else mouseState[keyName].callback = callback;
	
	std::stringstream stream;
	HayBCMD::Lexer lexer{callback};
	HayBCMD::Token token;
	while (token.getType() != HayBCMD::TokenType::_EOF) {
		if (token.getType() == HayBCMD::TokenType::COMMAND && token.getValue()[0] == '+') {
			stream << '-'+token.getValue().substr(1) << ";";
			
			while (token.getType() != HayBCMD::TokenType::_EOF && token.getType() != HayBCMD::TokenType::EOS)
				token = lexer.nextToken();
		}

		token = lexer.nextToken();
	}

	std::string offCallback = stream.str();
	offCallback = offCallback.substr(0, offCallback.size()-1);

	if (offCallback.size() == 0) return;

	if (isMouseWheelUp) // mouse wheel up
		mouseWheelUpCallback += ';'+offCallback;
	
	else if (isMouseWheelDown) // mouse wheel down
		mouseWheelDownCallback += ';'+offCallback;
	
	else if (isMouseButton) // mouse buttons
		mouseState[keyName].offCallback = offCallback;
	else // keyboard
		keyState[keyName].offCallback = offCallback;
}

void Meatball::Input::unbind(const std::string& keyName) {
	if (keyState.count(keyName) != 0) {
		keyState[keyName].callback = "";
		keyState[keyName].offCallback = "";
	
	} else if (mouseState.count(keyName) != 0) {
		mouseState[keyName].callback = "";
		mouseState[keyName].offCallback = "";
	
	} else if (keyName == "mwheelup")
		mouseWheelUpCallback = "";
	
	else if (keyName == "mwheeldown")
		mouseWheelDownCallback = "";
}

void Meatball::Input::bindCommand(HayBCMD::Command*, const std::vector<std::string>& args) {
	if (args.size() != 1) { // set
		bind(args[0], args[1]);
		return;
	}

	// get
	std::string callback;
	if (keyState.count(args[0]) != 0)
		callback = keyState[args[0]].callback;
	
	else if (mouseState.count(args[0]) != 0)
		callback = keyState[args[0]].callback;
	
	else if (args[0] == "mwheelup")
		callback = mouseWheelUpCallback;
	
	else if (args[0] == "mwheeldown")
		callback = mouseWheelDownCallback;
	else {
		Console::printf(HayBCMD::ERROR, "unknown key {}\n", args[0]);
		return;
	}

	Console::printf(HayBCMD::ECHO, "{} = \"{}\"\n", args[0], callback);
}

void Meatball::Input::unBindCommand(HayBCMD::Command*, const std::vector<std::string>& args) {
	unbind(args[0]);
}

void Meatball::Input::unBindAllCommand(HayBCMD::Command*, const std::vector<std::string>& args) {
	for (auto& key : keyState) {
		key.second.callback = "";
		key.second.offCallback = "";
	}

	for (auto& button : mouseState) {
		button.second.callback = "";
		button.second.offCallback = "";
	}

	mouseWheelUpCallback = mouseWheelDownCallback = "";
}

void Meatball::Input::registerCommands() {
	HayBCMD::Command("bind", 1, 2, bindCommand, "<key> <commands?> - binds a key to a command");
	HayBCMD::Command("unbind", 1, 1, unBindCommand, "<key> - unbinds a key");
	HayBCMD::Command("unbindall", 0, 0, unBindAllCommand, "unbinds all keys");
}

void Meatball::Input::setKey(const std::string& name, unsigned short code) {
	keyState[name] = {code, "", ""};
}

void Meatball::Input::removeKey(const std::string& name) {
	keyState.erase(name);
}

void Meatball::Input::setMouseKey(const std::string& name, unsigned short code) {
	mouseState[name] = {code, "", ""};
}

void Meatball::Input::removeMouseKey(const std::string& name) {
	mouseState.erase(name);
}

void Meatball::Input::update() {
	for (auto& key : keyState) {
		if (IsKeyPressed(key.second.code))
			Console::run(key.second.callback);
		
		else if (IsKeyReleased(key.second.code))
			Console::run(key.second.offCallback);
	}

	for (auto& button : mouseState) {
		if (IsKeyPressed(button.second.code))
			Console::run(button.second.callback);
		
		else if (IsKeyReleased(button.second.code)) 
			Console::run(button.second.offCallback);
	}

	float wheel = GetMouseWheelMove();
	if (wheel == 1.0f)
		Console::run(mouseWheelUpCallback);
	
	else if (wheel == -1.0f)
		Console::run(mouseWheelDownCallback);
}