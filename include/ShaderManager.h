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
		{"lambertian", 	"../assets/shaders/lambertian.vert", 		"../assets/shaders/lambertian.frag", Shader::Type::Foreground},
		{"normal", 		"../assets/shaders/normal.vert", 			"../assets/shaders/normal.frag", Shader::Type::Foreground},
		{"particle0", 	"../assets/shaders/particle_point.vert", 	"../assets/shaders/particle_point.frag", Shader::Type::Foreground},
		{"blinn-phong", "../assets/shaders/blinn-phong.vert", 		"../assets/shaders/blinn-phong.frag", Shader::Type::Foreground},
		{"basic", 		"../assets/shaders/basic_lighting.vert", 	"../assets/shaders/basic_lighting.frag", Shader::Type::Foreground},
		{"PBR0", 		"../assets/shaders/PBR0.vert", 				"../assets/shaders/PBR0.frag", Shader::Type::Foreground},
		{"matcap", 		"../assets/shaders/matcap.vert", 			"../assets/shaders/matcap.frag", Shader::Type::Foreground},
		{"bg", 			"../assets/shaders/bg_grad.vert", 			"../assets/shaders/bg_grad.frag", Shader::Type::Background},
		{"skybox", 		"../assets/shaders/skybox.vert", 			"../assets/shaders/skybox.frag", Shader::Type::Background},
		{"hdr2cubemap", "../assets/shaders/hdr2cubemap.vert", 		"../assets/shaders/hdr2cubemap.frag", Shader::Type::Background}, // ?
		{"grid", 		"../assets/shaders/gridShader.vert", 		"../assets/shaders/gridShader.frag", Shader::Type::Foreground}, // ? 
		{"selection", 	"../assets/shaders/selection.vert", 		"../assets/shaders/selection.frag", Shader::Type::Selection}, 
		

		//{"gouraud", "shaders/gouraud.vert", "shaders/gouraud.frag"},
		//{"flat", "shaders/flat.vert", "shaders/flat.frag"},
	};

public:
	void init();
	void terminate();
	Shader& getShader(const std::string& name);
};

