#pragma once

class DeltaTimer {
public:
	int last_time;
	float last_delta;
	float maximum;

	DeltaTimer(float maximum = 0.2f);
	float update();
};