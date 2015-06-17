#include <vector>
#include <glm/vec3.hpp>
#include <iostream>

using std::vector;
using glm::ivec3;

vector<ivec3> generate_grid(int nr, int nc) {
	vector<ivec3> triangles = vector<ivec3>(2*nr*nc);

	for (int ir = 0; ir < nr; ir++) {
		for (int ic = 0; ic < nc; ic++) {
			int it1 = 2*(ic + ir*nc) + 0;
			int it2 = 2*(ic + ir*nc) + 1;
			int iv00 = (ic + 0) + (ir + 0)*(nc + 1);
			int iv01 = (ic + 1) + (ir + 0)*(nc + 1);
			int iv10 = (ic + 0) + (ir + 1)*(nc + 1);
			int iv11 = (ic + 1) + (ir + 1)*(nc + 1);
			triangles[it1] = ivec3(iv00, iv10, iv11);
			triangles[it2] = ivec3(iv11, iv01, iv00);
		}
	}

	return triangles;
}