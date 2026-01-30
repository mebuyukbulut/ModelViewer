#pragma once
#include <glm/glm.hpp>
#include <unordered_map>
#include <memory>
#include "Asset.h"

class Texture;
class Shader;

class Material : public Asset
{
public:
	glm::vec4 baseColor;
	glm::vec4 emissive;
	float metallic;
	float roughness;
	float reflectance;
	float ao;

	std::shared_ptr<Texture> baseColorTexture{};
	std::shared_ptr<Texture> emissiveTexture{};
	std::shared_ptr<Texture> metallicTexture{};
	std::shared_ptr<Texture> roughnessTexture{};
	std::shared_ptr<Texture> aoTexture{};


	Material() :
		baseColor{ 1.0f, 1.0f, 1.0f, 1.0f },
		emissive{ 0.0f, 0.0f, 0.0f, 0.0f },
		metallic{ 0.0f },
		roughness{ 0.5f },
		reflectance{ 0.45f },
		ao{ 1.f }
	{}

	void use(Shader* shader);

	// Inherited via Asset
	void load(std::filesystem::path path, IAssetSettings settings) override;
	void unload() override;
	void uploadToGPU() override;

	virtual void onInspect();
};
