#pragma once

#include "lib/KeyState.h"
#include <vector>

using std::vector;

class Keyboard {
	vector<KeyState> now;
	vector<KeyState> next;
public:
	Keyboard(int size);
	bool pressed(int key);
	bool held(int key);
	bool released(int key);
	void press(int key);
	void release(int key);
	void update();
};