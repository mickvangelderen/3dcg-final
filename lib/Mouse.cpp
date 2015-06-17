#include "glut.h"
#include "lib/KeyState.h"
#include "lib/Mouse.h"

void Mouse::state(int button, int state) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) next_left.press();
		if (state == GLUT_UP) next_left.release();
		break;
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN) next_middle.press();
		if (state == GLUT_UP) next_middle.release();
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) next_right.press();
		if (state == GLUT_UP) next_right.release();
		break;
	}
}

void Mouse::position(int x, int y) {
	next_x = x;
	next_y = y;
}

void Mouse::update() {
	KeyState::update(left, next_left);
	KeyState::update(middle, next_middle);
	KeyState::update(right, next_right);
	dx = next_x - x;
	dy = next_y - y;
	x = next_x;
	y = next_y;
}