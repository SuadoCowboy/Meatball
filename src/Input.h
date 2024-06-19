#pragma once

#include <unordered_map>
#include <string>

#include <raylib.h>

namespace Meatball {
	struct KeyData {
		unsigned short code;
		std::string callback;
		std::string offCallback; // to turn off all the toggle types that might be in the callback
	};

	class Input {
	public:
		void bind(std::string keyName, const std::string& callback);
		void unbind(const std::string& keyName);
		
		/// @brief registers a key to be checked if is pressed or not
		/// @param name the name of the key ex: lalt
		/// @param code the raylib code of the key ex: KEY_LEFT_ALT
		void setKey(const std::string& name, unsigned short code);
		
		void removeKey(const std::string& name);

		/// @brief registers a key to be checked if is pressed or not
		/// @param name the name of the key ex: mouse1
		/// @param code the raylib code of the key ex: MOUSE_BUTTON_LEFT
		void setMouseKey(const std::string& name, unsigned short code);
		void removeMouseKey(const std::string& name);

		void update();

	private:
		static std::unordered_map<std::string, KeyData> keyState;
		static std::unordered_map<std::string, KeyData> mouseState;
		static KeyData mouseWheelUp;
		static KeyData mouseWheelDown;
	};
}