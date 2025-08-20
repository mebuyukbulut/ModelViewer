#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "Shader.h"

class ShaderManager
{
	struct ShaderInfo
	{
		std::string name;
		std::string vertexPath;
		std::string fragmentPath;
	};

	std::unordered_map<std::string, Shader> shaders;

	std::vector<ShaderInfo> shaderInfos{
		{"lambertian", "shaders/lambertian.vert", "shaders/lambertian.frag"},
		{"normal", "shaders/normal.vert", "shaders/normal.frag"},
		//{"phong", "shaders/phong.vert", "shaders/phong.frag"},
		{"blinn-phong", "shaders/blinn-phong.vert", "shaders/blinn-phong.frag"},
		{"basic", "shaders/basic_lighting.vert", "shaders/basic_lighting.frag"},
		//{"gouraud", "shaders/gouraud.vert", "shaders/gouraud.frag"},
		//{"flat", "shaders/flat.vert", "shaders/flat.frag"},
	};

public:
	void init();
	void terminate();
	Shader& getShader(const std::string& name);
};

