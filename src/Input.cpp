#include "Input.h"

MouseButton Meatball::Input::convertInpMouseButton(InpMouseButton button) {
	switch (button) {
	case InpMouseButton::MOUSE1:
		return MouseButton::MOUSE_BUTTON_LEFT;
	case InpMouseButton::MOUSE2:
		return MouseButton::MOUSE_BUTTON_RIGHT;
	case InpMouseButton::MOUSE3:
		return MouseButton::MOUSE_BUTTON_MIDDLE;
	case InpMouseButton::MOUSE4:
		return MouseButton::MOUSE_BUTTON_SIDE;
	case InpMouseButton::MOUSE5:
		return MouseButton::MOUSE_BUTTON_EXTRA;
	case InpMouseButton::MOUSE6:
		return MouseButton::MOUSE_BUTTON_FORWARD;
	case InpMouseButton::MOUSE7:
		return MouseButton::MOUSE_BUTTON_BACK;

	default:
		return MouseButton::MOUSE_BUTTON_LEFT; // ??? why would be default
	}
}

Meatball::Input::InpMouseButton Meatball::Input::convertInpMouseButton(MouseButton button) {
	switch (button) {
	case MouseButton::MOUSE_BUTTON_LEFT:
		return InpMouseButton::MOUSE1;
	case MouseButton::MOUSE_BUTTON_RIGHT:
		return InpMouseButton::MOUSE2;
	case MouseButton::MOUSE_BUTTON_MIDDLE:
		return InpMouseButton::MOUSE3;
	case MouseButton::MOUSE_BUTTON_SIDE:
		return InpMouseButton::MOUSE4;
	case MouseButton::MOUSE_BUTTON_EXTRA:
		return InpMouseButton::MOUSE5;
	case MouseButton::MOUSE_BUTTON_FORWARD:
		return InpMouseButton::MOUSE6;
	case MouseButton::MOUSE_BUTTON_BACK:
		return InpMouseButton::MOUSE7;

	default:
		return InpMouseButton::MOUSE1;
	}
}

unsigned char Meatball::Input::anyMouseButtonPressed() {
	unsigned char buttonsPressed = 0;
	for (unsigned char button = (unsigned char)MouseButton::MOUSE_BUTTON_LEFT; button < (unsigned char)MouseButton::MOUSE_BUTTON_BACK; button++)
		if (IsMouseButtonPressed(button))
			buttonsPressed |= convertInpMouseButton((MouseButton)button);
	return buttonsPressed;
}