#include "lib/model.h"

using std::cout;
using std::cin;
using std::endl;
using std::cerr;
using std::ifstream;
using std::ios;
using std::istringstream;

Model::Model(const char* filepath) {
	cout << "reading " << filepath << endl;
	loadMaterials(filepath);
	loadModel(filepath);
	cout << "Model has: " << faceMaterials.size() << " materials." << endl;
}

void Model::loadModel(const char* filepath) {
	string objpath = filepath + obj;
	Material activeMaterial;

	ifstream in(objpath, ios::in);
	if (!in) { cerr << "Cannot open OBJ file " << objpath << endl; exit(1); }

	string line;
	while (getline(in, line)) {
		if (line.substr(0,2) == "v ") {
			istringstream s(line.substr(2));
			vec3 v; s >> v.x; s >> v.y; s >> v.z;
			this->vertices.push_back(v);
		}  else if (line.substr(0,2) == "f ") {
			istringstream s(line.substr(2));
			ivec3 e; s >> e[0]; s >> e[1]; s >> e[2];
			this->triangles.push_back(e - 1);
			this->materials.push_back(activeMaterial);
		} else if (line.substr(0,7) == "usemtl ") {
			string matname;
			istringstream s(line.substr(7)); //newmtl Black
			s >> matname;
			activeMaterial = faceMaterials[matname];
		} else { /* ignoring this line */ }
	}

	cout << "Loaded: " << "v: " << this->vertices.size() << ", f: " << this->triangles.size() << endl;
	cout << "Materials: " << this->materials.size() << endl;

	this->normals.assign(this->vertices.size(), glm::vec3(0.0f));

	// Compute normals per element.
	for (vector<ivec3>::size_type it = 0; it < this->triangles.size(); it++) {
		ivec3 ivs = this->triangles[it];
		vec3 v1 = this->vertices[ivs[0]];
		vec3 v2 = this->vertices[ivs[1]];
		vec3 v3 = this->vertices[ivs[2]];
		vec3 n = glm::cross(v2 - v3, v2 - v1);
		this->normals[ivs[0]] += n;
		this->normals[ivs[1]] += n;
		this->normals[ivs[2]] += n;
	}

	// Normalize normals.
	for (vector<vec3>::size_type iv = 0; iv < this->normals.size(); iv++) {
		this->normals[iv] = glm::normalize(this->normals[iv]);
	}
}

void Model::loadMaterials(const char* filepath) {
	string mtlpath = filepath + mtl;

	ifstream in(mtlpath, ios::in);
	if (!in) { cerr << "Cannot open MTL file " << mtlpath << endl; exit(1); }

	string line;
	while (getline(in, line)) {
		//read material completely
		//not really fool-proof, try to load proper mtl files!
		if (line.substr(0,7) != "newmtl ") continue;

		Material m;

		istringstream s(line.substr(7)); //newmtl Black
		s >> m.name;

		getline(in, line); //get next line
		s.clear();
		s.str(line.substr(3)); //Ns 96.078431
		s >> m.ns;

		getline(in, line); //get next line
		s.clear();
		s.str(line.substr(3)); //Ka 0.000000 0.000000 0.000000
		s >> m.ka[0] >> m.ka[1] >> m.ka[2];

		getline(in, line); //get next line
		s.clear();
		s.str(line.substr(3)); //Kd 0.024571 0.024571 0.024571
		s >> m.kd[0] >> m.kd[1] >> m.kd[2];

		getline(in, line); //get next line
		s.clear();
		s.str(line.substr(3)); //Ks 0.500000 0.500000 0.500000
		s >> m.ks[0] >> m.ks[1] >> m.ks[2];

		getline(in, line); //get next line
		s.clear();
		s.str(line.substr(3)); //Ni 1.000000
		s >> m.ni;

		getline(in, line); //get next line
		s.clear();
		s.str(line.substr(2)); //d 1.000000
		s >> m.d;

		getline(in, line); //get next line
		s.clear();
		s.str(line.substr(6)); //illum 2
		s >> m.illum;

		faceMaterials[m.name] = m;
	}
}

void Model::draw() { //maybe add modelToView scale here as parameter?
	glBegin(GL_TRIANGLES);
	for (vector<ivec3>::size_type it = 0; it < this->triangles.size(); it++) {
		ivec3 ivs = this->triangles[it];
		for (int iv = 0; iv < 3; iv++) {
			glColor3fv(glm::value_ptr(this->materials[it].kd));
			glNormal3fv(glm::value_ptr(this->normals[ivs[iv]]));
			glVertex3fv(glm::value_ptr(this->vertices[ivs[iv]]));
		}
	}
	glEnd();
}