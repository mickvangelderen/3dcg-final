#pragma once

class DeltaTimer {
public:
	int last_time;
	float last_delta;
	float maximum;

	DeltaTimer(float maximum = 0.5f);
	float update();
};