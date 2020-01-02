#pragma once
#include <vector>
#include <glm\glm.hpp>
using std::vector;
using glm::vec3;
using glm::vec2;

struct Mesh {
	vector<vec3> vertices;
	vector<vec2> uvs;
	vector<unsigned int> indices;
	Mesh(){}
	Mesh(vector<vec3> vertices, vector<vec2> uvs, vector<unsigned int> indices) :
	vertices(std::move(vertices)), uvs(std::move(uvs)), indices(std::move(indices))
	{}
};