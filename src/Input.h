#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include <raylib.h>

#include <SweatCI.h>

namespace Meatball {
	struct InputData {
		std::string key;
		std::string callback;
		std::string offCallback; // to turn off all the toggle types that might be in the callback
		bool uiAllowed;
	};

	class Input {
	public:
		static void bind(std::string key, const std::string& callback);
		static void unbind(const std::string& key);

		/// @brief registers a key to be checked if is pressed or not
		/// @param key ex: lalt
		/// @param code the raylib code of the key ex: KEY_LEFT_ALT
		static void setKey(const std::string& key, unsigned short code);
		
		static void removeKey(const std::string& key);

		static void onMousePress(int button);
		static void onMouseRelease(int button);

		static void onKeyboardPress(int key);
		static void onKeyboardRelease(int key);

		static void onMouseWheel(const Vector2& dir);

		static void bindCommand(void*, SweatCI::Command&, const std::vector<std::string>& args);
		static void unBindCommand(void*, SweatCI::Command&, const std::vector<std::string>& args);
		static void unBindAllCommand(void*, SweatCI::Command&, const std::vector<std::string>& args);
		
		/// @brief register bind, unbind and unbindall commands
		static void registerCommands();

		/// @brief maps the keys of a US keyboard layout
		static void mapKeyboardKeys();

		/// @brief maps the keys of a 7 buttons mouse
		static void mapMouseKeys();

		static void setUiAllowedCommandsOnly(bool value);
		static bool isUiAllowedCommandsOnly();

		static std::vector<std::string> allowedUiCommands;

		static std::unordered_map<unsigned short, InputData> keysData;
		static std::unordered_map<unsigned short, InputData> mouseButtonsData;
		
		static std::string mouseWheelUpCallback;
		static std::string mouseWheelUpOffCallback;
		static std::string mouseWheelDownCallback;
		static std::string mouseWheelDownOffCallback;

	private:
		static unsigned char flags;
		/*
		1 = mouse wheel up
		2 = mouse wheel down
		4 = ui commands only - this is used when an interface is active, so that only ui allowed commands can run
		*/
	};
}