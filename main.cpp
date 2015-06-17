#if defined(_WIN32)
	#include <windows.h>
#endif
#include <GL/freeglut.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "lib/Keyboard.h"
#include "lib/DeltaTimer.h"
#include "lib/generate_grid.h"
#include "lib/generate_mountain.h"
#include <iostream>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>

using std::cout;
using std::endl;
using std::vector;
using glm::vec3;
using glm::vec4;
using glm::ivec2;
using glm::ivec3;
using glm::mat4;

vector<vec4> lights;
DeltaTimer deltaTimer;

Keyboard keyboard(256);

void initializeLights() {
	lights.resize(2);
	lights[0] = vec4( 1.0f, 1.0f, 3.0f, 1.0f);
	lights[1] = vec4(-2.0f, 4.0f, 5.0f, 1.0f);

	float w[] = { 1, 1, 0.8f, 1 };
	float b[] = { 0, 0.1f, 0.3f, 1 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, w);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, b);
}

void drawLight(const mat4 & transform, const vec4 & light) {
	if (light.w == 0.0f) return;
	mat4 local = glm::translate(transform, vec3(light)/light.w);
	glLoadMatrixf(glm::value_ptr(local));
	glPushAttrib(GL_LIGHTING);
	glDisable(GL_LIGHTING);
	glColor3f(1,1,0);
	glutSolidSphere(0.2, 8, 8);
	glPopAttrib();
	glLoadMatrixf(glm::value_ptr(transform));
}

void drawAxes(float length = 1) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glColor3f(1, 1, 1);
	glutSolidSphere(length/10.0f, 8, 8);
	glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(length, 0, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, length, 0);

		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, length);
	glEnd();
	glPopAttrib();
}

ivec2 surfaceSize(40, 40);
vec3 surfaceScale(8.0f, 8.0f, 2.0f);
vec3 surfacePosition(0.0f);
vector<float> surfaceHeights;
vector<vec3> surfaceVertices;
vector<ivec3> surfaceTriangles;
vector<vec3> surfaceNormals;
vector<vec3> surfaceColors;

void initializeSurface() {
	surfaceVertices.resize((surfaceSize.x + 1)*(surfaceSize.y + 1));
	surfaceColors.resize((surfaceSize.x + 1)*(surfaceSize.y + 1));
	surfaceTriangles = generate_grid(surfaceSize.x, surfaceSize.y);

	vector<MountainGenerationLevel> levels(3);
	levels[0].granularity = 0.1f; levels[0].min =  0.00f; levels[0].max = 1.00f;
	levels[1].granularity = 0.5f; levels[1].min = -0.10f; levels[1].max = 0.10f;
	levels[2].granularity = 1.0f; levels[2].min = -0.04f; levels[2].max = 0.04f;
	surfaceHeights = generate_mountain(surfaceSize.x + 1, surfaceSize.y + 1, levels);

	// Set vertex positions and colors.
	for (int ix = 0; ix <= surfaceSize.x; ix++) {
		for (int iy = 0; iy <= surfaceSize.y; iy++) {
			int iv = iy + ix*(surfaceSize.y + 1);
			float rx = (float) ix/surfaceSize.x - 0.5f;
			float ry = (float) iy/surfaceSize.y - 0.5f;
			float z = surfaceHeights[iv];
			surfaceVertices[iv] = vec3(rx, ry, z);

			if (z < 0.2) surfaceColors[iv] = vec3(113, 139, 77)/256.0f;
			else if (z < 0.4) surfaceColors[iv] = vec3(79, 101, 58)/256.0f;
			else if (z < 0.6) surfaceColors[iv] = vec3(166, 136, 100)/256.0f;
			else if (z < 0.8) surfaceColors[iv] = vec3(116, 118, 141)/256.0f;
			else surfaceColors[iv] = vec3(1, 1, 1);
		}
	}

	// Clear normals.
	surfaceNormals.assign((surfaceSize.x + 1)*(surfaceSize.y + 1), vec3(0.0f));

	// Compute normals per triangle.
	for (vector<ivec3>::size_type it = 0; it < surfaceTriangles.size(); it++) {
		ivec3 ivs = surfaceTriangles[it];
		vec3 v1 = surfaceVertices[ivs[0]];
		vec3 v2 = surfaceVertices[ivs[1]];
		vec3 v3 = surfaceVertices[ivs[2]];
		vec3 n = glm::cross(v2 - v3, v2 - v1);
		surfaceNormals[ivs[0]] += n;
		surfaceNormals[ivs[1]] += n;
		surfaceNormals[ivs[2]] += n;
	}

	// Normalize normals.
	for (vector<vec3>::size_type iv = 0; iv < surfaceNormals.size(); iv++) {
		surfaceNormals[iv] = glm::normalize(surfaceNormals[iv]);
	}
}

void drawSurface(const mat4 & transform) {
	glBegin(GL_TRIANGLES);
	for (vector<ivec3>::size_type it = 0; it < surfaceTriangles.size(); it++) {
		ivec3 ivs = surfaceTriangles[it];
		for (int iv = 0; iv < 3; iv++) {
			glColor3fv(glm::value_ptr(surfaceColors[ivs[iv]]));
			glNormal3fv(glm::value_ptr(surfaceNormals[ivs[iv]]));
			glVertex3fv(glm::value_ptr(surfaceVertices[ivs[iv]]));
		}
	}
	glEnd();
}

vec3 camera_position_velocity(0.0f, 0.0f, 0.0f);
vec3 camera_rotation_velocity(0.0f, 0.0f, 0.0f);
mat4 camera(1.0f);

void initializeCamera() {
	camera = glm::rotate(camera, glm::two_thirds<float>()*glm::pi<float>(), vec3(1.0f, 1.0f, 1.0f));
	camera = glm::inverse(camera);
	camera = glm::translate(camera, vec3(-4.0f, 0.0f, 0.0f));
	camera = glm::inverse(camera);
}

void render()
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);

	mat4 transform = glm::inverse(camera);

	glLoadMatrixf(glm::value_ptr(transform));

	for (vector<vec3>::size_type il = 0; il < lights.size(); il++) {
		glPushMatrix();
			glTranslatef(lights[il].x, lights[il].y, lights[il].z);
			glutSolidSphere(0.1, 8, 8);
		glPopMatrix();
		glLightfv(GL_LIGHT0 + il, GL_POSITION, glm::value_ptr(lights[il]));
		// drawLight(transform, lights[il]);
		// glLightfv(GL_LIGHT0 + il, GL_POSITION, glm::value_ptr(lights[il]));
	}

	// drawAxes();

	mat4 surfaceTransform = glm::scale(transform, surfaceScale);
	glLoadMatrixf(glm::value_ptr(surfaceTransform));
	drawSurface(transform);
	glLoadMatrixf(glm::value_ptr(transform));

	glutSwapBuffers();
	glutPostRedisplay();
}

float l1rot = 0;

void animate() {
	float delta = deltaTimer.update();
	keyboard.update();

	l1rot += delta;
	lights[0] = vec4(0.0f, 4*cos(l1rot), 4*sin(l1rot), 1.0f);

	vec3 crot(0.0f, 0.0f, 0.0f);
	if (keyboard.held('l')) crot.z = -delta;
	if (keyboard.held('j')) crot.z = delta;
	if (keyboard.held('i')) crot.x = delta;
	if (keyboard.held('k')) crot.x = -delta;
	if (keyboard.held('u')) crot.y = delta;
	if (keyboard.held('o')) crot.y = -delta;
	camera_rotation_velocity += crot;
	camera_rotation_velocity *= 0.9f;
	crot += 0.2f*camera_rotation_velocity;

	vec3 cmov(0.0f, 0.0f, 0.0f);
	if (keyboard.held('d')) cmov.x = delta;
	if (keyboard.held('a')) cmov.x = -delta;
	if (keyboard.held('w')) cmov.z = -delta;
	if (keyboard.held('s')) cmov.z = delta;
	if (keyboard.held('q')) cmov.y = delta;
	if (keyboard.held('z')) cmov.y = -delta;
	camera_position_velocity += cmov;
	camera_position_velocity *= 0.9f;
	cmov += 5.0f*camera_position_velocity;

	mat4 mcam(1.0f);
	mcam = glm::translate(mcam, cmov);
	mcam = glm::rotate(mcam, crot.x, vec3(1.0f, 0.0f, 0.0f));
	mcam = glm::rotate(mcam, crot.y, vec3(0.0f, 1.0f, 0.0f));
	mcam = glm::rotate(mcam, crot.z, vec3(0.0f, 0.0f, 1.0f));

	camera = camera*mcam;
}

//take keyboard input into account
void onKeyDown(unsigned char key, int x, int y) {
	keyboard.press(key);

	const char ESC = 27;

	if (key == ESC) exit(0);
}

void onKeyUp(unsigned char key, int x, int y) {
	keyboard.release(key);
}

void onSpecialKeyDown(int key, int x, int y) {
	// keys::onSpecialDown(key);

}

void onSpecialKeyUp(int key, int x, int y) {
	// keys::onSpecialUp(key);
}

int last_mouse_x;
int last_mouse_y;
float last_mouse_dx;
float last_mouse_dy;

void onMouse(int button, int state, int x, int y) {
	// cout << "Mouse press " << button << " state " << state << " x: " << x << " y: " << y << endl;

	last_mouse_x = x;
	last_mouse_y = y;

	if (state == 1) {
		vec3 cmov(-0.007f*last_mouse_dx, 0.007f*last_mouse_dy, 0.0f);
		camera_position_velocity = cmov;
	}
}


void onMotion(int x, int y) {
	// cout << "Mouse motion x: " << x << " y: " << y << endl;

	last_mouse_dx = x - last_mouse_x;
	last_mouse_dy = y - last_mouse_y;
	last_mouse_x = x;
	last_mouse_y = y;

	vec3 cmov(-0.007f*last_mouse_dx, 0.007f*last_mouse_dy, 0.0f);

	mat4 mcam(1.0f);
	mcam = glm::translate(mcam, cmov);

	camera = camera*mcam;
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho (-1.1, 1.1, -1.1,1.1, -1000.0, 1000.0);
	gluPerspective(50, (float)w/h, 1, 100);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
	// Initialize glut.
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(800,600);
	glutCreateWindow(argv[0]);

	// Initialize opengl.
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT,GL_FILL);
	glPolygonMode(GL_BACK,GL_LINE);

	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Initialize resources.
	initializeLights();
	initializeSurface();
	initializeCamera();

	// Configure glut.
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(onKeyDown);
	glutKeyboardUpFunc(onKeyUp);
	glutSpecialFunc(onSpecialKeyDown);
	glutSpecialUpFunc(onSpecialKeyUp);
	glutMouseFunc(onMouse);
	glutMotionFunc(onMotion);
	glutDisplayFunc(render);
	glutIdleFunc(animate);

	// Kickstart the main loop.
	glutMainLoop();

	return 0;
}
