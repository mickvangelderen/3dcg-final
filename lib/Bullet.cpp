#include "glut.h"
#include "lib/Bullet.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "lib/resources.h"

using glm::vec3;
using glm::mat4;

void Bullet::update(float delta) {
	age += delta;

	// Wind and gravity.
	velocity += delta*vec3(0.0f, -1.0f, -6.0f);

	// Apply friction.
	velocity *= 0.99f;

	// Update position;
	position += delta*velocity;
}

void Bullet::draw() {

	int it = 1; //default bullet without fire

	if (age < 0.2f){
		it = 4; //max fire
	} else if (age < 0.4f) {
		it = 5;
	} else if (age < 0.6f) {
		it = 6;
	} else if (age < 0.8f) {
		it = 7;
	}

	glBindTexture(GL_TEXTURE_2D, resources::textureIds[it]);

	glColor3f(0.2f, 0.1f, 0.1f);
	glutSolidSphere(0.1f, 4, 4);
}

void Bullet::render(const mat4 & transform) {
	mat4 local = transform;
	local = glm::translate(local, position);
	glLoadMatrixf(glm::value_ptr(local));
	draw();
}
