#pragma once
#include <glm/glm.hpp>

class Shader;

class Material
{
public:
	glm::vec4 baseColor;
	glm::vec4 emissive;
	float metallic;
	float roughness;
	float reflectance;
	float ao;

	Material() :
		baseColor{ 1.0,1.0,1.0,1.0 },
		emissive{ 0.0,0.0,0.0,0.0 },
		metallic{ 0.0 },
		roughness{ 0.5 },
		reflectance{ 0.45 },
		ao{ 1 }
	{}
	//Material(glm::vec4 baseColor, float roughness): 
	//	baseColor{ baseColor }, 
	//	roughness{ roughness } {
	//}

	void use(Shader* shader);
	void drawUI();
};

