#pragma once
#include <vector>
#include <glm/glm.hpp>

struct Vertex
{
	float x, y, z; // position
	//glm::vec3 position;
};

class Model
{
	std::vector<Vertex> _vertices;
	std::vector<unsigned int> _indices;

	unsigned int _vao, _vbo, _ebo;
	void setupMesh();
public:

	void loadFromFile(const char* filename);
	void loadDefault();
	void draw();
	void use();
	void terminate();
	//void update(float deltaTime);
};

