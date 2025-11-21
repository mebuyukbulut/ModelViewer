#pragma once
#include <string>
#include <vector>

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
	static Texture* load(const char* path, const std::string& directory, bool gamma);
	static Texture* loadCubeMap(std::vector<std::string> faces);
};

