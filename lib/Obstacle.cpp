#include "glut.h"
#include "lib/Obstacle.h"
#include "lib/resources.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

using glm::vec3;
using glm::mat4;

void Obstacle::update(float delta) {
	// Update position;
	position += delta*velocity;
}

void Obstacle::draw() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_TEXTURE_2D);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, resources::textureIds[0]);

		//box coords
		static GLfloat normals[6][3] = {
			{-1.0, 0.0, 0.0},
			{0.0, 1.0, 0.0},
			{1.0, 0.0, 0.0},
			{0.0, -1.0, 0.0},
			{0.0, 0.0, 1.0},
			{0.0, 0.0, -1.0}
		};

		static GLint faces[6][4] = {
			{0, 1, 2, 3},
			{3, 2, 6, 7},
			{7, 6, 5, 4},
			{4, 5, 1, 0},
			{5, 6, 2, 1},
			{7, 4, 0, 3}
		};

		GLfloat vertices[8][3];
		vertices[0][0] = vertices[1][0] = vertices[2][0] = vertices[3][0] = -0.5f;
		vertices[4][0] = vertices[5][0] = vertices[6][0] = vertices[7][0] = 0.5f;
		vertices[0][1] = vertices[1][1] = vertices[4][1] = vertices[5][1] = -0.5f;
		vertices[2][1] = vertices[3][1] = vertices[6][1] = vertices[7][1] = 0.5f;
		vertices[0][2] = vertices[3][2] = vertices[4][2] = vertices[7][2] = -0.5f;
		vertices[1][2] = vertices[2][2] = vertices[5][2] = vertices[6][2] = 0.5f;

		for (int i = 5; i >= 0; i--) {
			glBegin(GL_QUADS);
				glNormal3fv(&normals[i][0]);

				glTexCoord2f(0, 0);
				glVertex3fv(&vertices[faces[i][0]][0]);

				glTexCoord2f(1, 0);
				glVertex3fv(&vertices[faces[i][1]][0]);

				glTexCoord2f(1, 1);
				glVertex3fv(&vertices[faces[i][2]][0]);

				glTexCoord2f(0, 1);
				glVertex3fv(&vertices[faces[i][3]][0]);
			glEnd();
		}
	glPopAttrib();
}

void Obstacle::render(const mat4 & transform) {
	mat4 local = transform;
	local = glm::translate(local, position);
	local = glm::scale(local, scale);
	glLoadMatrixf(glm::value_ptr(local));
	draw();
}
