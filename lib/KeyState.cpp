#include "KeyState.h"

KeyState & KeyState::press() {
	pressed = true;
	held = true;
	return (*this);
}

KeyState & KeyState::release() {
	released = true;
	held = false;
	return (*this);
}

void KeyState::update(KeyState & now, KeyState & next) {
	now = next;
	next.pressed = false;
	next.released = false;
}