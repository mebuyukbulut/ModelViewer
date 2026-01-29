#pragma once
#include <string>
#include <vector>
#include "Asset.h"

class Shader;

class Texture : public Asset {
public:
	Texture();
	Texture(unsigned int id, unsigned int type);
	unsigned int _id; 
	unsigned int _type; // OpenGL Enum
	std::string _path;
	void use();
	void terminate();


	// Inherited via Asset
	void load(std::filesystem::path path, IAssetSettings settings) override;

	void unload() override;

	void uploadToGPU() override;

};

class TextureFactory {
public:
	static Texture* load(const std::string& filename, bool gamma);
	static Texture* loadCubeMap(std::vector<std::string> faces);
	static Texture* loadHDR(std::string path, Shader* shader);
};

