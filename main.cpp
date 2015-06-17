#if defined(_WIN32)
	#include <windows.h>
#endif
#include <math.h>
#include "glut.h"
#include "lib/Bullet.h"
#include "lib/DeltaTimer.h"
#include "lib/generate_grid.h"
#include "lib/generate_mountain.h"
#include "lib/Keyboard.h"
#include "lib/Model.h"
#include "lib/Mouse.h"
#include "lib/resources.h"
#include <assert.h>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

using glm::ivec2;
using glm::ivec3;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using std::cout;
using std::endl;
using std::vector;
using std::list;

DeltaTimer deltaTimer;
Keyboard keyboard(256);
Keyboard special(256);
Mouse mouse;

// Axes

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

// lightPositions.

vector<vec4> lightPositions;
vector<vec4> lightDiffuses;
vector<float> lightIntensities;

void initializelightPositions() {
	lightPositions.resize(3);
	lightPositions[0] = vec4( 1.0f, 1.0f, 3.0f, 1.0f);
	lightPositions[1] = vec4(-2.0f, 4.0f, 5.0f, 1.0f);
	lightPositions[2] = vec4(0.0f, 0.4f, 0.6f, 1.0f);
	lightDiffuses.resize(3);
	lightDiffuses[0] = vec4( 1, 1, 0.8f, 1 );
	lightDiffuses[1] = vec4( 0, 0.1f, 0.3f, 1 );
	lightDiffuses[2] = vec4( 1.0f, 0.8f, 0.3f, 1 );
	lightIntensities.resize(3, 1.0f);
	lightIntensities[1] = 0.8f;
	lightIntensities[2] = 0.4f;
	glLighti(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.0f);
	glLighti(GL_LIGHT2, GL_LINEAR_ATTENUATION, 1.0f);
	// glLighti(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.0f);
}

void drawLight(int index, const vec4 position, const vec4 & diffuse, float intensity) {
	glLightfv(GL_LIGHT0 + index, GL_DIFFUSE, glm::value_ptr(intensity*diffuse));
	glLightfv(GL_LIGHT0 + index, GL_POSITION, glm::value_ptr(position));
	if (!keyboard.held('/')) return;
	if (position.w == 0.0f) return;
	glPushMatrix();
	glPushAttrib(GL_LIGHTING);
		glDisable(GL_LIGHTING);
		glTranslatef(position.x/position.w, position.y/position.w, position.z/position.w);
		glColor3fv(glm::value_ptr(vec3(diffuse)));
		glutSolidSphere(1, 8, 8);
	glPopAttrib();
	glPopMatrix();
}

// Text (score, game over, etc)
void drawText(const char *string)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(2,-2,2.9);
    glRotatef(-90, 0, 0, 1);
    glRotatef(90, 1, 0, 0);
    glRotatef(180, 0, 1, 0);
    glScalef(0.001,0.001,0.001);
    glColor3f(0,0,0);
    for(unsigned int i = 0; i<strlen(string); i++){
        glutStrokeCharacter(GLUT_STROKE_ROMAN,string[i]);
    }
    glPopAttrib();
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

float score = 0;

void renderGameInfo() {
	std::stringstream ss;
    ss << "Distance travelled: " << round(score) << " bananas";
    std::string s = ss.str();
    const char* cstr = s.c_str();

    drawText(cstr);
}

// Surface.

vec3 surfacePosition(0.0f, 0.0f, 0.0f);
vec3 surfaceScale(5.0f, 16.0f, 4.0f);
vec2 surfaceView(0.0f, 0.0f);
ivec2 surfaceSize(50, 160);
ivec2 surfaceHeightsSize(51, 801);
vector<float> surfaceHeights;
vector<vec3> surfaceVertices;
vector<ivec3> surfaceTriangles;
vector<vec3> surfaceNormals;
vector<vec3> surfaceColors;

void initializeSurface() {
	surfaceVertices.resize((surfaceSize.x + 1)*(surfaceSize.y + 1));
	surfaceColors.resize((surfaceSize.x + 1)*(surfaceSize.y + 1));
	surfaceTriangles = generate_grid(surfaceSize.x, surfaceSize.y);

	vector<MountainGenerationLevel> levels(4);
	levels[0].granularity = 0.05f; levels[0].min =  0.00f; levels[0].max = 1.00f;
	levels[1].granularity = 0.20f; levels[1].min = -0.20f; levels[1].max = 0.20f;
	levels[2].granularity = 0.50f; levels[2].min = -0.04f; levels[2].max = 0.04f;
	levels[3].granularity = 1.00f; levels[3].min = -0.02f; levels[3].max = 0.02f;
	surfaceHeights = generate_mountain(surfaceHeightsSize.x, surfaceHeightsSize.y, levels);
}

void drawSurface() {
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

void renderSurface(const mat4 & transform) {

	// Set vertex positions and colors.
	for (int ix = 0; ix <= surfaceSize.x; ix++) {
		for (int iy = 0; iy <= surfaceSize.y; iy++) {
			// Compute vertex index.
			int iv = iy + ix*(surfaceSize.y + 1);

			// Compute view coordinates.
			int ixview = (int) surfaceView.x;
			int iyview = (int) surfaceView.y;
			float offsetx = (surfaceView.x - ixview)/surfaceSize.x;
			float offsety = (surfaceView.y - iyview)/surfaceSize.y;

			// Compute height map index.
			int ixh = (ix + ixview) % surfaceHeightsSize.x;
			int iyh = (iy + iyview) % surfaceHeightsSize.y;
			int ih = iyh + ixh*surfaceHeightsSize.y;

			// Compute x and y coordinates relative to the surface.
			float rsx = (float) ix/surfaceSize.x;
			float rsy = (float) iy/surfaceSize.y;

			float c = (glm::cos(rsx*glm::pi<float>()) + 1.0f)/2.0f; // Flatten 1 side.
			float z = c*surfaceHeights[ih];
			if (z < 0.0f) z = 0.0f;
			surfaceVertices[iv] = vec3(rsx - offsetx - 0.5, rsy - offsety - 0.5, z);

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

	mat4 local = glm::translate(transform, surfacePosition);
	local = glm::scale(local, surfaceScale);
	glLoadMatrixf(glm::value_ptr(local));
	drawSurface();
}

// Floor.

vec3 floorPosition(4.0f, 0.0f, -0.0001f);
vec3 floorScale(6.0f, 16.0f, 1.0f);
vec3 floorColor = vec3(113, 139, 77)/256.0f;
ivec2 floorSize(60, 160);
vector<vec3> floorVertices;
vector<ivec3> floorTriangles;

void initializeFloor() {
	floorVertices.resize((floorSize.x + 1)*(floorSize.y + 1));
	floorTriangles = generate_grid(floorSize.x, floorSize.y);

	// Set vertex positions and colors.
	for (int ix = 0; ix <= floorSize.x; ix++) {
		for (int iy = 0; iy <= floorSize.y; iy++) {
			// Compute vertex index.
			int iv = iy + ix*(floorSize.y + 1);

			// Compute x and y coordinates relative to the surface.
			float rsx = (float) ix/floorSize.x;
			float rsy = (float) iy/floorSize.y;
			floorVertices[iv] = vec3(rsx - 0.5f, rsy - 0.5f, 0);
		}
	}
}

void drawFloor() {
	glColor3fv(glm::value_ptr(floorColor));
	glNormal3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	for (vector<ivec3>::size_type it = 0; it < floorTriangles.size(); it++) {
		ivec3 ivs = floorTriangles[it];
		for (int iv = 0; iv < 3; iv++) {
			glVertex3fv(glm::value_ptr(floorVertices[ivs[iv]]));
		}
	}
	glEnd();
}

void renderfloor(const mat4 & transform) {
	mat4 local = glm::translate(transform, floorPosition);
	local = glm::scale(local, floorScale);
	glLoadMatrixf(glm::value_ptr(local));
	glColor3fv(glm::value_ptr(floorColor));
	drawFloor();
}

// Player.

vec3 playerPosition(4.0f, 0.0f, 0.5f);
vec3 playerMinPosition(-100.0f, -100.0f, 00.5f);
vec3 playerMaxPosition( 100.0f,  100.0f, 20.0f);
vec3 playerRotation(0.0f);
vec3 playerVelocity(0.0f);
float playerSwag = 0.0f;
vec3 playerScale(0.2f);
Model daveTheMinion = Model("models/dave2");
Model daveTheMinionFlapping = Model("models/dave2-flap");

void initializePlayer() {

}

void drawPlayer() {
	if (keyboard.held(' ')) {
		daveTheMinionFlapping.draw();
	} else {
		daveTheMinion.draw();
	}
}

void drawPlayerLeg() {
    glColor3f(0.024571,0.024571,0.024571);
	glutSolidCube(1);
}

void renderPlayerLeg(const mat4 & transform, float direction) {
	mat4 local = transform;
	local = glm::rotate(local, glm::sin(direction*playerSwag)*0.5f, vec3(1.0f, 0.0f, 0.0f));
	local = glm::scale(local, vec3(0.08f, 0.08f, 0.24f));
	local = glm::translate(local, vec3(0.0f, 0.0f, -0.5f));
	glLoadMatrixf(glm::value_ptr(local));
	drawPlayerLeg();
}

void renderPlayer(const mat4 & transform) {
	mat4 local = glm::translate(transform, playerPosition);
	local = glm::rotate(local, playerRotation.x, vec3(1.0f, 0.0f, 0.0f));
	local = glm::rotate(local, playerRotation.y, vec3(0.0f, 1.0f, 0.0f));
	local = glm::rotate(local, playerRotation.z, vec3(0.0f, 0.0f, 1.0f));
	glLoadMatrixf(glm::value_ptr(local));
	drawLight(2, lightPositions[2], lightDiffuses[2], lightIntensities[2]);
	renderPlayerLeg(glm::translate(local, vec3(-0.06f, 0.0f, -0.2f)), -1.0f);
	renderPlayerLeg(glm::translate(local, vec3( 0.06f, 0.0f, -0.2f)),  1.0f);
	// Model changes.
	local = glm::rotate(local, glm::sin(playerSwag)*0.2f, vec3(0.2f, 0.0f, 1.0f));
	local = glm::rotate(local, glm::cos(playerSwag)*0.15f, vec3(0.0f, 0.4f, 1.0f));
	local = glm::scale(local, playerScale);
	local = glm::rotate(local, glm::half_pi<float>(), vec3(1.0f, 0.0f, 0.0f));
	local = glm::rotate(local, glm::pi<float>(), vec3(0.0f, 1.0f, 0.0f));
	glLoadMatrixf(glm::value_ptr(local));
	drawPlayer();
}

// Bullets.

list<Bullet> bullets;

// Camera.

vec3 camera_position_velocity(0.0f, 0.0f, 0.0f);
vec3 camera_rotation_velocity(0.0f, 0.0f, 0.0f);
mat4 camera(1.0f);

void initializeCamera() {
	camera = glm::rotate(camera, glm::two_thirds<float>()*glm::pi<float>(), vec3(1.0f, 1.0f, 1.0f));
	camera = glm::inverse(camera);
	camera = glm::rotate(camera, 0.1f, vec3(0.0f, 1.0f, 0.0f)); // Pitch forward.
	camera = glm::translate(camera, vec3(-8.0f, 0.0f, -1.0f)); // Move back and up.
	camera = glm::inverse(camera);
}

// Rendering

float daynight = 0;

void render() {
	vec3 day = vec3(94, 127, 232)/256.0f;
	vec3 night = vec3(5, 12, 22)/256.0f;
	vec3 clear = glm::mix(night, day, (glm::sin(daynight) + 1.0f)/2.0f);
	glClearColor(clear.r, clear.g, clear.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);

	renderGameInfo();

	mat4 transform = glm::inverse(camera);
	glLoadMatrixf(glm::value_ptr(transform));

	if (keyboard.held('/')) drawAxes();

	renderSurface(transform);

	renderfloor(transform);

	renderPlayer(transform);

	

	for (list<Bullet>::iterator ib = bullets.begin(); ib != bullets.end(); ++ib) {
		ib->render(transform);
	}

	glLoadMatrixf(glm::value_ptr(transform));
	drawLight(0, lightPositions[0], lightDiffuses[0], lightIntensities[0]);
	drawLight(1, lightPositions[1], lightDiffuses[1], lightIntensities[1]);

	glutSwapBuffers();
	glutPostRedisplay();
}

// Animation.

void animate() {
	float delta = deltaTimer.update();
	score += delta * 3;

	keyboard.update();
	special.update();
	mouse.update();

	// Update surface position.
	// surfacePosition += 1.0f*vec3(0.0f, delta, 0.0f);
	surfaceView += 2.0f*vec2(0.0f, delta);

	// Update day/night cycle.
	daynight += delta*0.4f;
	lightPositions[0] = vec4(0.0f, 8*glm::cos(daynight), 8*glm::sin(daynight), 1.0f);
	lightIntensities[0] = (glm::sin(daynight) + 1.0f)/2.0f;

	// Player movement.
	if (keyboard.pressed(' ')) playerVelocity += vec3(0.0f, 0.0f, 4.0f);
	playerVelocity += delta*vec3(0.0f, 0.0f, -12.0f);
	playerVelocity *= 0.95;
	playerPosition += delta*playerVelocity;
	if (playerPosition.z < playerMinPosition.z) {
		playerPosition.z = playerMinPosition.z;
		playerVelocity.z = glm::max(playerVelocity.z, 0.0f);
	}
	if (playerPosition.z > playerMaxPosition.z) {
		playerPosition.z = playerMaxPosition.z;
		playerVelocity.z = glm::min(playerVelocity.z, 0.0f);
	}
	playerRotation.x = atan2(playerVelocity.z, 4);

	// Player legs.
	if (playerPosition.z == playerMinPosition.z) playerSwag += delta*8.0f;

	// Mouse movement.
	vec3 camMouseMov(-0.007f*mouse.dx, 0.007f*mouse.dy, 0.0f);
	if (mouse.left.released) {
		camera_position_velocity = camMouseMov;
	} else if (mouse.left.held) {
		mat4 mcam(1.0f);
		mcam = glm::translate(mcam, camMouseMov);
		camera = camera*mcam;
	}

	// Fire a bullet.
	if (keyboard.pressed('m')) {
		Bullet bullet;
		bullet.position = playerPosition + vec3(0.0f, 0.5f, 0.0f);
		bullet.velocity = 6.0f*vec3(0.0f, glm::cos(playerRotation.x), glm::sin(playerRotation.x));
		bullets.push_back(bullet);
	}

	// Update bullets.
	for (list<Bullet>::iterator ib = bullets.begin(); ib != bullets.end();) {
		ib->update(delta);
		if (ib->age < 5.0f) ++ib;
		else ib = bullets.erase(ib);
	}

	// Update camera.
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

void onKeyDown(unsigned char key, int x, int y) {
	keyboard.press(key);

	const char ESC = 27;

	if (key == ESC) exit(0);
	if (key == '1') glEnable(GL_LIGHT0);
	if (key == '!') glDisable(GL_LIGHT0);
	if (key == '2') glEnable(GL_LIGHT1);
	if (key == '@') glDisable(GL_LIGHT1);
	if (key == '3') glEnable(GL_LIGHT2);
	if (key == '#') glDisable(GL_LIGHT2);
}

void onKeyUp(unsigned char key, int x, int y) {
	keyboard.release(key);
}

void onSpecialKeyDown(int key, int x, int y) {
	special.press(key);
}

void onSpecialKeyUp(int key, int x, int y) {
	special.release(key);
}

void onMouse(int button, int state, int x, int y) {
	mouse.state(button, state);
}


void onMotion(int x, int y) {
	mouse.position(x, y);
}

void onPassiveMotion(int x, int y) {
	mouse.position(x, y);
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
	glEnable(GL_LIGHT2);
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
	resources::initializeTextures();
	initializelightPositions();
	initializeSurface();
	initializeFloor();
	initializePlayer();
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
	glutPassiveMotionFunc(onPassiveMotion);
	glutDisplayFunc(render);
	glutIdleFunc(animate);

	// Kickstart the main loop.
	glutMainLoop();

	return 0;
}