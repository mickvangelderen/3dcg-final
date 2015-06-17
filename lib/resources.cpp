#include "glut.h"
#include "lib/loadppm.h"
#include "lib/resources.h"
#include <vector>

using std::vector;

namespace resources {
	vector<GLuint> textureIds;

	void initializeTextures() {
		const char* filenames[] = { "textures/mountain.ppm", "textures/bullet.ppm", "textures/grass.ppm",
									"textures/clouds4.ppm", "textures/bullet-fire1.ppm", "textures/bullet-fire2.ppm",
									"textures/bullet-fire3.ppm", "textures/bullet-fire4.ppm", "textures/brick.ppm" };

		textureIds.resize(9);
		glGenTextures(9, &textureIds[0]);
		for (int i = 0; i < 9; i++) {
			PPMImage image(filenames[i]);
			glBindTexture(GL_TEXTURE_2D, textureIds[i]);
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image.sizeX, image.sizeY, GL_RGB, GL_UNSIGNED_BYTE, image.data);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}