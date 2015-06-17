#include "glut.h"
#include "DeltaTimer.h"

DeltaTimer::DeltaTimer(float maximum) {
	this->maximum = maximum;
	last_time = 0;
}

float DeltaTimer::update() {
	int now = glutGet(GLUT_ELAPSED_TIME);
	last_delta = (float) (now - last_time)/1000.0f;
	if (last_delta < 0.0f) last_delta = 0;
	if (last_delta > maximum) last_delta = maximum;
	last_time = now;
	return last_delta;
}