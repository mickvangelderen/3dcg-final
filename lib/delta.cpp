#include <GL/freeglut.h>

namespace delta {

	int old;

	void init() {
		old = glutGet(GLUT_ELAPSED_TIME);
	}

	float update() {
		int now = glutGet(GLUT_ELAPSED_TIME);
		float change = now - old;
		old = now;
		return (change < 500 ? change : 500)/1000;
	}

}