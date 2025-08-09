#pragma once
#include <glm/glm.hpp>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

class Mesh
{
	std::vector<Vertex> _vertices;
	std::vector<unsigned int> _indices;

	unsigned int _vao, _vbo, _ebo;
	void setupMesh();
public:

	void loadFromFile(const char* filename);
	void loadDefault();
	void init(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices);
	void draw();
	void terminate();
	//void update(float deltaTime);

};

