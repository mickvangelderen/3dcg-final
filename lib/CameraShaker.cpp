#include "lib/CameraShaker.h"
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

using glm::vec3;

void CameraShaker::fire() {
	elapsed = 0.0f;
}

void CameraShaker::update(float delta) {
	elapsed += delta;
}

vec3 CameraShaker::translation() {
	float x = elapsed/length;
	float amp;
	if (x > 1.0f) {
		amp = 0.0f;
	} else {
		const float PI = glm::pi<float>();
		amp = (glm::cos(PI*x) + 1)/2*glm::sin(shakes*2*PI*x);
	}
	return amp*direction;
}