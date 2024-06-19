#include "Input.h"

#include <algorithm>
#include <cctype>
#include <sstream>

#include "Console.h"

void Meatball::Input::bind(std::string keyName, const std::string& callback) {
	std::transform(keyName.begin(), keyName.end(), keyName.begin(),
    [](unsigned char c){ return std::tolower(c); });

	if (keyState.count(keyName) == 0) return;
	
	keyState[keyName].callback = callback;
	
	std::stringstream stream;
	HayBCMD::Lexer lexer{callback};
	HayBCMD::Token token;
	while (token.getType() != HayBCMD::TokenType::_EOF) {
		if (token.getType() == HayBCMD::TokenType::COMMAND && token.getValue()[0] == '+') {
			stream << token.getValue() << ";";
			
			while (token.getType() != HayBCMD::TokenType::_EOF && token.getType() != HayBCMD::TokenType::EOS)
				token = lexer.nextToken();
		}

		token = lexer.nextToken();
	}

	keyState[keyName].offCallback = stream.str();
	keyState[keyName].offCallback = keyState[keyName].offCallback.substr(0, keyState[keyName].offCallback.size()-1);
}

void Meatball::Input::unbind(const std::string& keyName) {
	if (keyState.count(keyName) == 0) return;

	keyState[keyName].callback = "";
}

void Meatball::Input::setKey(const std::string& name, unsigned short code) {
	keyState[name] = {code, ""};
}

void Meatball::Input::removeKey(const std::string& name) {
	keyState.erase(name);
}

void Meatball::Input::setMouseKey(const std::string& name, unsigned short code) {
	mouseState[name] = {code, ""};
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
}