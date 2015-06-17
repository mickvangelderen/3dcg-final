#include <GL/freeglut.h>
#include "DeltaTimer.h"

DeltaTimer::DeltaTimer(float maximum) {
	this->maximum = maximum;
	last_time = glutGet(GLUT_ELAPSED_TIME);
}

float DeltaTimer::update() {
	int now = glutGet(GLUT_ELAPSED_TIME);
	last_delta = (float) (now - last_time)/1000.0f;
	if (last_delta > maximum) last_delta = maximum;
	last_time = now;
	return last_delta;
}