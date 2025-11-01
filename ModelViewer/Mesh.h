#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <string>
#include "Shader.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
typedef OpenMesh::PolyMesh_ArrayKernelT<>  OMesh;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
	std::vector<Vertex> _vertices;
	std::vector<unsigned int> _indices;
	std::vector<Texture> _textures;

	unsigned int _vao, _vbo, _ebo;
	void setupMesh();
public:

	//void loadFromFile(const char* filename);
	//void loadDefault();
	void init(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void init(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	void draw(Shader& shader);
	void terminate();
	//void update(float deltaTime);

};



// Use factory for model and mesh and create primitive objects 

enum class DefaultShapes
{
	Cube,
	Cylinder,
	Cone,
	Sphere,
	Plane,
};


class MeshFactory {
	static OMesh createCube();
	static OMesh createCone();
	static OMesh createCylinder();
	static OMesh createPlane();

public:
	static Mesh create(DefaultShapes shape);
};