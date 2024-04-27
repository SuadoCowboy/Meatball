#pragma once

#include <unordered_map>
#include <string>

#include <raylib.h>

namespace Meatball {
	/*
	enum MOUSEBUTTONS { // I don't think it's necessary to recreate a enum
		MOUSE1 = MouseButton::MOUSE_BUTTON_LEFT,
		MOUSE2 = MouseButton::MOUSE_BUTTON_RIGHT,
		MOUSE3 = MouseButton::MOUSE_BUTTON_MIDDLE,
		MOUSE4 = MouseButton::MOUSE_BUTTON_BACK,
		MOUSE5 = MouseButton::MOUSE_BUTTON_FORWARD,
		MOUSE6 = MouseButton::MOUSE_BUTTON_SIDE
	};
	*/
	class Input {
	public:
		static bool pressed();
		static bool pressedOnce();

		static bool released();
		static bool releasedOnce();

	private:
		std::unordered_map<std::string, short> keys; // (keyname, keycode)
	}
}