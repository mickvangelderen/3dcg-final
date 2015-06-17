#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

using glm::mat4;
using glm::vec3;

class Obstacle {
public:
	vec3 position;
	vec3 velocity;
	vec3 scale;

	void update(float delta);
	void draw();
	void render(const mat4 & transform);
};
