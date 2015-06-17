#include "generate_mountain.h"
#include <math.h>
#include <vector>
#include <glm/gtc/random.hpp>

using std::vector;

vector<float> generate_mountain(int nx, int ny, vector<MountainGenerationLevel> levels) {
	vector<float> m(nx*ny, 0);

	for (int li = 0; li < (int) levels.size(); li++) {
		MountainGenerationLevel level = levels[li];

		// Compute number of sections.
		int sx = (int) (level.granularity*nx);
		int sy = (int) (level.granularity*ny);

		if (sx <= 0 || sy <= 0) continue;

		// Initialize values at the corners of each section.
		vector<float> v((sx + 1)*(sy + 1));

		for (int vx = 0; vx < sx; vx++) {
			for (int vy = 0; vy < sy; vy++) {
				int vi = vx + vy*(sx + 1);
				v[vi] = glm::linearRand(level.min, level.max);
			}
		}

		// Wrap values in the x direction excluding the last row.
		for (int vy = 0; vy < (sy + 0); vy++) {
			v[sx + vy*(sx + 1)] = v[0 + vy*(sx + 1)];
		}

		// Wrap values in the y direction including the last column.
		for (int vx = 0; vx < (sx + 1); vx++) {
			v[vx + sy*(sx + 1)] = v[vx + 0*(sx + 1)];
		}

		// Compute the values at each point in the mountain grid using
		// bilinear interpolation.
		for (int mx = 0; mx < nx; mx++) {
			float xif = ((float) mx)*((float) sx)/((float) nx);
			int xi = (int) xif;
			float xid = xif - ((float) xi);
			for (int my = 0; my < ny; my++) {
				float yif = ((float) my)*((float) sy)/((float) ny);
				int yi = (int) yif;
				float yid = yif - ((float) yi);
				float v00 = v[xi + 0 + (yi + 0)*(sx + 1)];
				float v01 = v[xi + 0 + (yi + 1)*(sx + 1)];
				float v10 = v[xi + 1 + (yi + 0)*(sx + 1)];
				float v11 = v[xi + 1 + (yi + 1)*(sx + 1)];
				m[mx + my*nx] +=
					v00*(1 - xid)*(1 - yid) +
					v01*(1 - xid)*(0 + yid) +
					v10*(0 + xid)*(1 - yid) +
					v11*(0 + xid)*(0 + yid);
			}
		}
	}

	return m;
}
