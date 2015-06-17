#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

using glm::mat4;
using glm::vec3;

class Bullet {
public:
	vec3 position;
	vec3 rotation;
	vec3 velocity;
	bool flying = true;
	float age = 0;

	void update(float delta);
	void draw();
	void render(const mat4 & transform);
};