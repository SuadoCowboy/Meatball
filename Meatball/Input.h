#pragma once

#include <unordered_map>
#include <string>

#include <raylib.h>

namespace Meatball {
	namespace Input {
		// WARNING: do not use this enum for raylib functions. ONLY USE ON INPUT NAMESPACE FUNCTIONS
		// why this enum exists? simple: I want to anyMouseButtonPressed
		// to return correctly when more than 1 button is pressed.
		enum InpMouseButton {
			// I don't think it's necessary to recreate a enum -> SHUT UP, NOW IT IS!
			MOUSE1 = 1,  // left
			MOUSE2 = 2,  // right
			MOUSE3 = 4,  // middle
			MOUSE4 = 8,  // side
			MOUSE5 = 16, // side extra
			MOUSE6 = 32, // I don't actually know and I don't give a fuck. FUCK YOU MOUSE6
			MOUSE7 = 64, // have no idea what button this is.
		};

		// InpMouseButton -> Raylib MouseButton enum
		// NOT TO BE CONFUSED WITH THE OTHER WAY AROUND!
		// It's TWO functions! Same name, opposite direction
		MouseButton convertInpMouseButton(InpMouseButton button);
		// Raylib MouseButton enum -> InpMouseButton
		// NOT TO BE CONFUSED WITH THE OTHER WAY AROUND!
		// It's TWO functions! Same name, opposite direction
		InpMouseButton convertInpMouseButton(MouseButton button);

		// WARNING: read return
		// @return InpMouseButton with all the pressed buttons together bitwise; 0 if no button pressed
		unsigned char anyMouseButtonPressed();
	}

	// I don't know what to do with input.h... Maybe handle binds?
}