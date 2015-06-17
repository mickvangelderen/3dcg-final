#include "lib/Keyboard.h"
#include "lib/KeyState.h"
#include <vector>

using std::vector;

Keyboard::Keyboard(int size) {
	now.resize(size);
	next.resize(size);
}

bool Keyboard::pressed(int key) {
	return now[key].pressed;
}

bool Keyboard::held(int key) {
	return now[key].held;
}

bool Keyboard::released(int key) {
	return now[key].released;
}

void Keyboard::press(int key) {
	next[key].pressed = true;
	next[key].held = true;
}

void Keyboard::release(int key) {
	next[key].released = true;
	next[key].held = false;
}

void Keyboard::update() {
	for (vector<KeyState>::size_type key = 0; key < now.size(); key++) {
		now[key] = next[key];
		next[key].pressed = false;
		next[key].released = false;
	}
}