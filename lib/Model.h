#pragma once

#include "glut.h"
#include "lib/Material.h"
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using glm::ivec3;
using glm::vec3;
using glm::vec4;
using std::map;
using std::string;
using std::vector;

class Model {
public:
	vector<vec3> vertices;
	vector<vec3> normals;
	vector<ivec3> triangles;
	vector<Material> materials;

	map<string, Material> faceMaterials;

	Model(const char *filepath);

	void draw(); //maybe add modelToView scale here as parameter?

private:
	void loadModel(const char *filepath);
	void loadMaterials(const char *filepath);
	string obj = ".obj";
	string mtl = ".mtl";
};
