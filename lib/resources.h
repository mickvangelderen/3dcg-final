#pragma once

#include "glut.h"
#include <vector>

using std::vector;

namespace resources {
	extern vector<GLuint> textureIds;
	void initializeTextures();
}