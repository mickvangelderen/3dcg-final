#pragma once

#include "lib/KeyState.h"

class Mouse {
private:
	KeyState next_left, next_middle, next_right;
	int next_x = 0, next_y = 0;
public:
	KeyState left, middle, right;
	int x = 0, dx = 0;
	int y = 0, dy = 0;

	void state(int button, int state);
	void position(int x, int y);
	void update();
};