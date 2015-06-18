#pragma once

#include <glm/vec3.hpp>

using glm::vec3;

class CameraShaker {
public:
	vec3 direction = vec3(0.0f, 1.0f, 0.0f);
	float length = 1.0f;
	float elapsed = 1.0f;
	float shakes = 3.0f;

	void fire();

	void update(float delta);

	vec3 translation();
};