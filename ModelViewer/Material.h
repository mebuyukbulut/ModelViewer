#pragma once
#include <glm/glm.hpp>
#include <unordered_map>
#include <memory>

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
		baseColor{ 1.0f, 1.0f, 1.0f, 1.0f },
		emissive{ 0.0f, 0.0f, 0.0f, 0.0f },
		metallic{ 0.0f },
		roughness{ 0.5f },
		reflectance{ 0.45f },
		ao{ 1.f }
	{}
	//Material(glm::vec4 baseColor, float roughness): 
	//	baseColor{ baseColor }, 
	//	roughness{ roughness } {
	//}

	void use(Shader* shader);
	void drawUI();
};


struct MaterialHandle {
	int id = -1;
	bool isValid() const { return id >= 0; }
};

class MaterialManager {

public:
	MaterialManager(Shader* shader) :_shader{ shader } {}

	MaterialHandle createMaterial();
	Material* getMaterial(MaterialHandle handle);
	MaterialHandle getDefaultMaterial();
	void useMaterial(MaterialHandle handle);
	void destroyMaterial(MaterialHandle handle);

private:
	Shader* _shader; 
	int nextId = 0;
	std::unordered_map<int, std::unique_ptr<Material>> materials;
};

