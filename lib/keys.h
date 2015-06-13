#pragma once

#include <vector>

namespace keys {

	using std::vector;

	bool held(unsigned char key);
	bool held(int key);
	void onDown(unsigned char key);
	void onUp(unsigned char key);
	void onSpecialDown(int key);
	void onSpecialUp(int key);
}
