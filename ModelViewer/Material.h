#pragma once
#include <glm/glm.hpp>

class Shader;

class Material
{
public:
	glm::vec4 _color;
	float _specular;

	Material() : _color{ 1,1,1,1 }, _specular{0.5}{}
	Material(glm::vec4 color, float specular)
		: _color{ color }, _specular{ specular } {}

	void use(Shader* shader);
	void drawUI();
};

