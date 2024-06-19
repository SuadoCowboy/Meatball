#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include <raylib.h>

#include <HayBCMD.h>

namespace Meatball {
	struct InputData {
		unsigned short code;
		std::string callback;
		std::string offCallback; // to turn off all the toggle types that might be in the callback
	};

	class Input {
	public:
		static void bind(std::string keyName, const std::string& callback);
		static void unbind(const std::string& keyName);

		/// @brief registers a key to be checked if is pressed or not
		/// @param name the name of the key ex: lalt
		/// @param code the raylib code of the key ex: KEY_LEFT_ALT
		static void setKey(const std::string& name, unsigned short code);
		
		static void removeKey(const std::string& name);

		/// @brief registers a key to be checked if is pressed or not
		/// @param name the name of the key ex: mouse1
		/// @param code the raylib code of the key ex: MOUSE_BUTTON_LEFT
		static void setMouseKey(const std::string& name, unsigned short code);
		static void removeMouseKey(const std::string& name);

		static void update();

		static void bindCommand(HayBCMD::Command*, const std::vector<std::string>& args);
		static void unBindCommand(HayBCMD::Command*, const std::vector<std::string>& args);
		static void unBindAllCommand(HayBCMD::Command*, const std::vector<std::string>& args);
		
		static void registerCommands();

		/// @brief maps the keys of a US keyboard layout
		static void mapKeyboardKeys();

		/// @brief maps the keys of a 7 buttons mouse
		static void mapMouseKeys();

	private:
		static std::unordered_map<std::string, InputData> keyState;
		static std::unordered_map<std::string, InputData> mouseState;
		
		static std::string mouseWheelUpCallback;
		static std::string mouseWheelUpOffCallback;
		static std::string mouseWheelDownCallback;
		static std::string mouseWheelDownOffCallback;

		static unsigned char mouseWheelRolled;
		/*
		1 = up
		2 = down
		*/
	};
}