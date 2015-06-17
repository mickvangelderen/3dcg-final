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
	next[key].press();
}

void Keyboard::release(int key) {
	next[key].release();
}

void Keyboard::update() {
	for (vector<KeyState>::size_type key = 0; key < now.size(); key++) {
		KeyState::update(now[key], next[key]);
	}
}