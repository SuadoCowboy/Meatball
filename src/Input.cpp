#include "Input.h"

#include <algorithm>
#include <cctype>
#include <sstream>

#include "Console.h"

std::vector<std::string> Meatball::Input::allowedUiCommands;

std::unordered_map<std::string, Meatball::InputData> Meatball::Input::keyState;
std::unordered_map<std::string, Meatball::InputData> Meatball::Input::mouseState;

std::string Meatball::Input::mouseWheelUpCallback;
std::string Meatball::Input::mouseWheelUpOffCallback;

std::string Meatball::Input::mouseWheelDownCallback;
std::string Meatball::Input::mouseWheelDownOffCallback;

unsigned char Meatball::Input::mouseWheelRolled;

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
	else if (keyState.count(keyName) == 0 && !isMouseButton) {
		Console::printf(HayBCMD::ERROR, "unknown key \"{}\"", keyName);
		return;
	}
	
	if (!isMouseButton) keyState[keyName].callback = callback;
	else mouseState[keyName].callback = callback;
	
	std::stringstream stream;
	HayBCMD::Lexer lexer{callback};
	HayBCMD::Token token = lexer.nextToken();
	while (token.getType() != HayBCMD::TokenType::_EOF) {
		if (token.getType() == HayBCMD::TokenType::COMMAND && std::find(allowedUiCommands.begin(), allowedUiCommands.end(), token.getValue()) == allowedUiCommands.end()) {
			if (isMouseButton)
				mouseState[keyName].uiAllowed = false;
			else if (!isMouseWheelDown && !isMouseWheelUp)
				keyState[keyName].uiAllowed = false;
		}

		if (token.getValue()[0] == '+') {
			if (token.getType() != HayBCMD::TokenType::COMMAND && Console::variables.count(token.getValue()) == 0) {
				token = lexer.nextToken();
				continue;
			}

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
		mouseWheelUpOffCallback = offCallback;
	
	else if (isMouseWheelDown) // mouse wheel down
		mouseWheelDownOffCallback = offCallback;
	
	else if (isMouseButton) // mouse buttons
		mouseState[keyName].offCallback = offCallback;
	else // keyboard
		keyState[keyName].offCallback = offCallback;
}

void Meatball::Input::unbind(const std::string& keyName) {
	if (keyState.count(keyName) != 0) {
		keyState[keyName].callback = "";
		keyState[keyName].offCallback = "";
		keyState[keyName].uiAllowed = true;
	
	} else if (mouseState.count(keyName) != 0) {
		mouseState[keyName].callback = "";
		mouseState[keyName].offCallback = "";
		keyState[keyName].uiAllowed = true;
	
	} else if (keyName == "mwheelup")
		mouseWheelUpCallback = "";
	
	else if (keyName == "mwheeldown")
		mouseWheelDownCallback = "";
}

void Meatball::Input::setKey(const std::string& name, unsigned short code) {
	if (name.size() > 5 && name.substr(0, 5) == "mouse")
		mouseState[name] = {code, "", "", true};
	else
		keyState[name] = {code, "", "", true};
}

void Meatball::Input::removeKey(const std::string& name) {
	keyState.erase(name);
}

void Meatball::Input::update(bool uiAllowedCommandsOnly) {
	for (auto& key : keyState) {
		if (!key.second.uiAllowed && uiAllowedCommandsOnly) continue;

		if (IsKeyPressed(key.second.code))
			Console::run(key.second.callback);
		else if (IsKeyReleased(key.second.code))
			Console::run(key.second.offCallback);
	}

	for (auto& button : mouseState) {
		if (!button.second.uiAllowed && uiAllowedCommandsOnly) continue;

		if (IsMouseButtonPressed(button.second.code))
			Console::run(button.second.callback);
		
		else if (IsMouseButtonReleased(button.second.code)) 
			Console::run(button.second.offCallback);
	}

	if (uiAllowedCommandsOnly) return;

	float wheel = GetMouseWheelMove();
	if (wheel == 1.0f) {
		Console::run(mouseWheelUpCallback);
		mouseWheelRolled |= 1;
	}
	
	else if (wheel == -1.0f) {
		Console::run(mouseWheelDownCallback);
		mouseWheelRolled |= 2;
	}

	else {
		if (mouseWheelRolled & 1) {
			Console::run(mouseWheelUpOffCallback);
			mouseWheelRolled &= ~1;
		} else if (mouseWheelRolled & 2) {
			Console::run(mouseWheelDownOffCallback);
			mouseWheelRolled &= ~2;
		}
	}
}

void Meatball::Input::bindCommand(void*, HayBCMD::Command&, const std::vector<std::string>& args) {
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

void Meatball::Input::unBindCommand(void*, HayBCMD::Command&, const std::vector<std::string>& args) {
	unbind(args[0]);
}

void Meatball::Input::unBindAllCommand(void*, HayBCMD::Command&, const std::vector<std::string>&) {
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

void Meatball::Input::mapKeyboardKeys() {
    // A-Z
	for (unsigned char i = KEY_A; i <= KEY_Z; ++i)
        setKey(std::string(1, static_cast<char>(i+32)), i);
    
	// left & right
	setKey("lalt", KEY_LEFT_ALT);
	setKey("ralt", KEY_RIGHT_ALT);
	setKey("[", KEY_LEFT_BRACKET);
	setKey("]", KEY_RIGHT_BRACKET);
	setKey("lctrl", KEY_LEFT_CONTROL);
	setKey("rctrl", KEY_RIGHT_CONTROL);
	setKey("lshift", KEY_LEFT_SHIFT);
	setKey("rshift", KEY_RIGHT_SHIFT);
	setKey("lsuper", KEY_LEFT_SUPER);
	setKey("rsuper", KEY_RIGHT_SUPER);
    setKey("leftarrow", KEY_LEFT);
    setKey("rightarrow", KEY_RIGHT);
    setKey("uparrow", KEY_UP);
    setKey("downarrow", KEY_DOWN);
	
	// KeyPad
	for (unsigned short i = KEY_KP_0; i <= KEY_KP_9; ++i)
		setKey("kp_"+std::to_string(i-KEY_KP_0), i);
	
	setKey("kp_add", KEY_KP_ADD);
	setKey("kp_decimal", KEY_KP_DECIMAL);
	setKey("kp_divide", KEY_KP_DIVIDE);
	setKey("kp_enter", KEY_KP_ENTER);
	setKey("kp_equal", KEY_KP_EQUAL);
	setKey("kp_multiply", KEY_KP_MULTIPLY);
	setKey("kp_subtract", KEY_KP_SUBTRACT);

	// Digits
	for (unsigned short i = KEY_ZERO; i <= KEY_NINE; ++i)
		setKey(std::to_string(i-KEY_ZERO), i);

	// Function keys
	for (unsigned short i = KEY_F1; i <= KEY_F12; ++i)
		setKey("f"+std::to_string(i-KEY_F1+1), i);

	// Others (alphabetical order)
	setKey("'", KEY_APOSTROPHE);

	setKey("\\", KEY_BACKSLASH);
	setKey("backspace", KEY_BACKSPACE);
	
	setKey("capslock", KEY_CAPS_LOCK);
	setKey(",", KEY_COMMA);
	
	setKey("delete", KEY_DELETE);
	
	setKey("", KEY_END);
	setKey("enter", KEY_ENTER);
	setKey("equal", KEY_EQUAL);
	setKey("escape", KEY_ESCAPE);

	setKey("`", KEY_GRAVE);
	
	setKey("home", KEY_HOME);

	setKey("insert", KEY_INSERT);

	setKey("kb_menu", KEY_KB_MENU); // should this be keypad? what is that key?
	
	setKey("menu", KEY_MENU);
	setKey("-", KEY_MINUS);

	setKey("numlock", KEY_NUM_LOCK);

	setKey("pagedown", KEY_PAGE_DOWN);
	setKey("pageup", KEY_PAGE_UP);
	setKey("pause", KEY_PAUSE);
	setKey(".", KEY_PERIOD);
	setKey("printscreen", KEY_PRINT_SCREEN);

	setKey("scrolllock", KEY_SCROLL_LOCK); // lll haha
	setKey(";", KEY_SEMICOLON);
	setKey("/", KEY_SLASH);
	setKey("space", KEY_SPACE);

	setKey("tab", KEY_TAB);

	setKey("volumedown", KEY_VOLUME_DOWN);
	setKey("volumeup", KEY_VOLUME_UP);
}

void Meatball::Input::mapMouseKeys() {
	for (unsigned short i = MOUSE_BUTTON_LEFT; i < MOUSE_BUTTON_BACK; ++i)
		setKey("mouse"+std::to_string(i+1), i);
}