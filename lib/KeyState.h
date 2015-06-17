#pragma once

class KeyState {
public:
	bool pressed = false;
	bool held = false;
	bool released = false;

	KeyState & press();
	KeyState & release();
	static void update(KeyState & now, KeyState & next);
};