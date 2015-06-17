#pragma once

namespace keys {
	bool held(unsigned char key);
	bool held(int key);
	void onDown(unsigned char key);
	void onUp(unsigned char key);
	void onSpecialDown(int key);
	void onSpecialUp(int key);
}
