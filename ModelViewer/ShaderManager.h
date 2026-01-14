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
		Shader::Type type;
	};

	std::unordered_map<std::string, Shader> shaders;

	std::vector<ShaderInfo> shaderInfos{
		{"lambertian", "shaders/lambertian.vert", "shaders/lambertian.frag", Shader::Type::Foreground},
		{"normal", "shaders/normal.vert", "shaders/normal.frag", Shader::Type::Foreground},
		{"particle0", "shaders/particle_point.vert", "shaders/particle_point.frag", Shader::Type::Foreground},
		{"blinn-phong", "shaders/blinn-phong.vert", "shaders/blinn-phong.frag", Shader::Type::Foreground},
		{"basic", "shaders/basic_lighting.vert", "shaders/basic_lighting.frag", Shader::Type::Foreground},
		{"PBR0", "shaders/PBR0.vert", "shaders/PBR0.frag", Shader::Type::Foreground},
		{"bg", "shaders/bg_grad.vert", "shaders/bg_grad.frag", Shader::Type::Background},
		{"skybox", "shaders/skybox.vert", "shaders/skybox.frag", Shader::Type::Background},
		{"hdr2cubemap", "shaders/hdr2cubemap.vert", "shaders/hdr2cubemap.frag", Shader::Type::Background}, // ?
		{"grid", "shaders/gridShader.vert", "shaders/gridShader.frag", Shader::Type::Foreground}, // ? 
		{"selection", "shaders/selection.vert", "shaders/selection.frag", Shader::Type::Selection}, 
		

		//{"gouraud", "shaders/gouraud.vert", "shaders/gouraud.frag"},
		//{"flat", "shaders/flat.vert", "shaders/flat.frag"},
	};

public:
	void init();
	void terminate();
	Shader& getShader(const std::string& name);
};

