#include "Input.h"

#include <algorithm>
#include <cctype>
#include <sstream>

#include "Console.h"

std::vector<std::string> Meatball::Input::allowedUiCommands;

std::unordered_map<unsigned short, Meatball::InputData> Meatball::Input::keysData;
std::unordered_map<unsigned short, Meatball::InputData> Meatball::Input::mouseButtonsData;

std::string Meatball::Input::mouseWheelUpCallback;
std::string Meatball::Input::mouseWheelUpOffCallback;

std::string Meatball::Input::mouseWheelDownCallback;
std::string Meatball::Input::mouseWheelDownOffCallback;

unsigned char Meatball::Input::flags;

static void errorUnknownKey(const std::string& key) {
	Meatball::Console::printf(SweatCI::ERROR, "Unknown key: \"{}\"\n", key);
}

static bool beginsWithMouse(const std::string& str) {
	return str.size() >= 5 && str.substr(0, 5) == "mouse";
}

void Meatball::Input::bind(std::string key, const std::string& callback) {
	std::transform(key.begin(), key.end(), key.begin(),
    [](unsigned char c){ return std::tolower(c); });

	bool uiAllowed = true;
	std::stringstream stream;
	SweatCI::Lexer lexer{callback};
	SweatCI::Token token = lexer.nextToken();
	while (token.getType() != SweatCI::TokenType::_EOF) {
		if (token.getType() == SweatCI::TokenType::COMMAND && std::find(allowedUiCommands.begin(), allowedUiCommands.end(), token.getValue()) == allowedUiCommands.end()) {
			uiAllowed = false;
		}

		if (token.getValue()[0] == '+') {
			if (token.getType() != SweatCI::TokenType::COMMAND && Console::variables.count(token.getValue()) == 0) {
				token = lexer.nextToken();
				continue;
			}

			stream << '-'+token.getValue().substr(1) << ";";
			
			while (token.getType() != SweatCI::TokenType::_EOF && token.getType() != SweatCI::TokenType::EOS)
				token = lexer.nextToken();
		}

		token = lexer.nextToken();
	}

	std::string offCallback = stream.str();
	offCallback = offCallback.substr(0, offCallback.size()-1);

	for (auto& button : mouseButtonsData)
		if (button.second.key == key) {
			button.second.callback = callback;
			button.second.offCallback = offCallback;
			button.second.uiAllowed = uiAllowed;
			return;
		}

	if (key == "mwheelup") {
		mouseWheelUpCallback = callback;
		mouseWheelUpOffCallback = offCallback;
		return;
	
	} else if (key == "mwheeldown") {
		mouseWheelDownCallback = callback;
		mouseWheelDownOffCallback = offCallback;
		return;
	
	} else for (auto& keyData: keysData)
		if (keyData.second.key == key) {
			keyData.second.callback = callback;
			keyData.second.offCallback = offCallback;
			keyData.second.uiAllowed = uiAllowed;
			return;
		}
	
	errorUnknownKey(key);
}

void Meatball::Input::unbind(const std::string& key) {
	if (key == "mwheelup") {
		mouseWheelUpCallback = "";
		mouseWheelUpOffCallback = "";
		return;
	} else if (key == "mwheeldown") {
		mouseWheelDownCallback = "";
		mouseWheelDownOffCallback = "";
		return;
	}

	if (beginsWithMouse(key)) {
		for (auto& button : mouseButtonsData)
			if (key == button.second.key) {
				button.second.callback = "";
				button.second.offCallback = "";
				button.second.uiAllowed = true;
				return;
			}
	} else for (auto& keyData : keysData)
		if (keyData.second.key == key) {
			keyData.second.callback = "";
			keyData.second.offCallback = "";
			keyData.second.uiAllowed = true;
			return;
		}
	
	errorUnknownKey(key);
}

void Meatball::Input::setKey(const std::string& key, unsigned short code) {
	if (beginsWithMouse(key)) {
		mouseButtonsData[code] = {};
		mouseButtonsData[code].key = key;
	} else {
		keysData[code] = {};
		keysData[code].key = key;
	}
}

void Meatball::Input::removeKey(const std::string& key) {
	for (auto& keyData : keysData)
		if (keyData.second.key == key) {
			keysData.erase(keyData.first);
			return;
		}
}

void Meatball::Input::onMousePress(int buttonId) {
	if (mouseButtonsData.count(buttonId) == 0 || (!mouseButtonsData[buttonId].uiAllowed && (flags & 4))) return;

	Console::run(mouseButtonsData[buttonId].callback);
}

void Meatball::Input::onMouseRelease(int buttonId) {
	if (mouseButtonsData.count(buttonId) == 0 || (!mouseButtonsData[buttonId].uiAllowed && (flags & 4))) return;

	Console::run(mouseButtonsData[buttonId].offCallback);
}

void Meatball::Input::onKeyboardPress(int keyId, bool isRepeat) { 
	if (isRepeat || keysData.count(keyId) == 0 || (!keysData[keyId].uiAllowed && (flags & 4))) return;

	Console::run(keysData[keyId].callback);
}

void Meatball::Input::onKeyboardRelease(int keyId) {
	if (keysData.count(keyId) == 0 || (!keysData[keyId].uiAllowed && (flags & 4))) return;

	Console::run(keysData[keyId].offCallback);
}

void Meatball::Input::onMouseWheel(const Vector2& dir) {
	if (flags & 4) return; // if (uiAllowedCommandsOnly) return;

	if (dir.y == 1.0f) {
		Console::run(mouseWheelUpCallback);
		flags |= 1;
	}
	
	else if (dir.y == -1.0f) {
		Console::run(mouseWheelDownCallback);
		flags |= 2;
	}

	else {
		if (flags & 1) {
			Console::run(mouseWheelUpOffCallback);
			flags &= ~1;
		} else if (flags & 2) {
			Console::run(mouseWheelDownOffCallback);
			flags &= ~2;
		}
	}
}

void Meatball::Input::bindCommand(void*, SweatCI::Command&, const std::vector<std::string>& args) {
	if (args.size() != 1) { // set
		bind(args[0], args[1]);
		return;
	}

	// get
	bool isUnknownKey = true;
	std::string callback;
	if (beginsWithMouse(args[0])) {
		for (auto& button : mouseButtonsData)
			if (button.second.key == args[0]) {
				callback = button.second.callback;
				isUnknownKey = false;
				break;
			}
	
	} else if (args[0] == "mwheelup") {
		callback = mouseWheelUpCallback;
		isUnknownKey = false;
	
	} else if (args[0] == "mwheeldown") {
		callback = mouseWheelDownCallback;
		isUnknownKey = false;
	
	} else for (auto& keyData : keysData)
		if (keyData.second.key == args[0]) {
			callback = keyData.second.callback;
			isUnknownKey = false;
			break;
		}
	
	if (isUnknownKey) {
		errorUnknownKey(args[0]);
		return;
	}

	Console::printf(SweatCI::ECHO, "{} = \"{}\"\n", args[0], callback);
}

void Meatball::Input::unBindCommand(void*, SweatCI::Command&, const std::vector<std::string>& args) {
	unbind(args[0]);
}

void Meatball::Input::unBindAllCommand(void*, SweatCI::Command&, const std::vector<std::string>&) {
	for (auto& key : keysData) {
		key.second.callback = "";
		key.second.offCallback = "";
	}

	for (auto& button : mouseButtonsData) {
		button.second.callback = "";
		button.second.offCallback = "";
	}

	mouseWheelUpCallback = mouseWheelUpOffCallback = mouseWheelDownCallback = mouseWheelDownOffCallback = "";
}

void Meatball::Input::registerCommands() {
	SweatCI::Command("bind", 1, 2, bindCommand, "<key> <commands?> - binds a key to a command");
	SweatCI::Command("unbind", 1, 1, unBindCommand, "<key> - unbinds a key");
	SweatCI::Command("unbindall", 0, 0, unBindAllCommand, "unbinds all keys");
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
	for (unsigned short code = MOUSE_BUTTON_LEFT; code < MOUSE_BUTTON_BACK; ++code)
		setKey("mouse"+std::to_string(code+1), code);
}

void Meatball::Input::setUiAllowedCommandsOnly(bool value) {
	if (value)
		flags |= 4;
	else
		flags &= ~4;
}

bool Meatball::Input::isUiAllowedCommandsOnly() {
	return flags & 4;
}
