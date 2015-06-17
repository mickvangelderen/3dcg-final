#include "glut.h"
#include "lib/Bullet.h"
#include "lib/resources.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

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

	if (age < 0.04f){
		it = 4; //max fire
	} else if (age < 0.08f) {
		it = 5;
	} else if (age < 0.12f) {
		it = 6;
	} else if (age < 0.16f) {
		it = 7;
	}

	glPushAttrib(GL_ALL_ATTRIB_BITS);
    	glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, resources::textureIds[it]);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_LINEAR);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_LINEAR);

		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(0.0f, 1.0f); //top-left
			glVertex2f(-0.5f, 0.152235f);

			glTexCoord2f(0.0f, 0.0f); //bottom-left
			glVertex2f(-0.5f, -0.152235f);

			glTexCoord2f(0.74581f, 1.0f);
			glVertex2f(0.3729f, 0.152235f);

			glTexCoord2f(0.74581f, 0.0f);
			glVertex2f(0.3729f, -0.152235f);

			glTexCoord2f(0.9581f, 0.761467f);
			glVertex2f(0.47905f, 0.07961f);

			glTexCoord2f(0.9581f, 0.238532f);
			glVertex2f(0.47905f, -0.07961f);

			glTexCoord2f(1.0f, 0.5f); //middle-right (end of bullet)
			glVertex2f(0.5f, 0.0f);
		glEnd();
	glPopAttrib();
}

void Bullet::render(const mat4 & transform) {
	mat4 local = transform;
	local = glm::translate(local, position);
	local = glm::rotate(local, glm::atan(velocity.z, velocity.y), vec3(1.0f, 0.0f, 0.0f));
	// Model.
	local = glm::rotate(local, glm::half_pi<float>(), vec3(1.0f, 0.0f, 0.0f));
	local = glm::rotate(local, glm::half_pi<float>(), vec3(0.0f, 1.0f, 0.0f));
	local = glm::scale(local, vec3(0.4f, 0.4f, 0.4f));
	glLoadMatrixf(glm::value_ptr(local));
	draw();
}
