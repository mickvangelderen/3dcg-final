#pragma once

#include <vector>

using std::vector;

struct MountainGenerationLevel {
	float granularity;
	float min;
	float max;
};

vector<float> generate_mountain(int nx, int ny, vector<MountainGenerationLevel> levels);
