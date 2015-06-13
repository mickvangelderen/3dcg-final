#include <vector>

namespace keys {

	using std::vector;

	vector<bool> normal(256, false);
	vector<bool> special(256, false);

	bool held(unsigned char key) {
		return normal[key];
	}

	bool held(int key) {
		return special[key];
	}

	void onDown(unsigned char key) {
		normal[key] = true;
	}

	void onUp(unsigned char key) {
		normal[key] = false;
	}

	void onSpecialDown(int key) {
		special[key] = true;
	}

	void onSpecialUp(int key) {
		special[key] = false;
	}

}