#pragma once

#include "glm/vec3.hpp"
#include <string>

using std::string;
using glm::vec3;

/* holding the following
newmtl Black
Ns 96.078431
Ka 0.000000 0.000000 0.000000
Kd 0.024571 0.024571 0.024571
Ks 0.500000 0.500000 0.500000
Ni 1.000000
d 1.000000
illum 2
*/

class Material {

public:
	string name;
	float ns;
	vec3 ka;
	vec3 kd;
	vec3 ks;
	float ni;
	float d;
	int illum;
};
