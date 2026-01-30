#pragma once
#include <string>
#include <vector>
#include "Asset.h"

class Shader;

class Texture : public Asset {
	unsigned char* _data{};
	int _nrChannels, _width, _height; 

	unsigned int _id; 
	unsigned int _type; // OpenGL Enum

public:
	void use();

	// Inherited via Asset
	void load(std::filesystem::path path, IAssetSettings settings) override;
	void unload() override;
	void uploadToGPU() override;

};

//class TextureFactory {
//public:
//	static Texture* loadCubeMap(std::vector<std::string> faces);
//	static Texture* loadHDR(std::string path, Shader* shader);
//};
//
