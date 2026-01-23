#pragma once
#include <string>
#include <vector>

class Shader;

class Texture {
public:
	Texture();
	Texture(unsigned int id, unsigned int type);
	unsigned int _id; 
	unsigned int _type; // OpenGL Enum
	std::string _path;
	void use();
	void terminate();

};
class TextureFactory {
public:
	static Texture* load(const std::string& filename, bool gamma);
	static Texture* loadCubeMap(std::vector<std::string> faces);
	static Texture* loadHDR(std::string path, Shader* shader);
};

