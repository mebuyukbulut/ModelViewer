#pragma once
#include <string>
#include <glm/glm.hpp>
#include <unordered_map>

class Shader
{
	unsigned int _shaderProgram;
	std::unordered_map<std::string, unsigned int> _uniforms;

	unsigned int compileShader(std::string shaderCode, unsigned int shaderType);
	unsigned int linkProgram(unsigned int vertexShader, unsigned int fragmentShader);
	unsigned int getUniformLocation(const std::string& name);

public:
	void init(std::string vertexShaderSource, std::string fragmentShaderSource);
	void use(); 
	void terminate(); 

	void setMat4(const std::string& name, const glm::mat4& mat);
	void setVec3(const std::string& name, const glm::vec3& vec);
	void setFloat(const std::string& name, float value);
	void setInt(const std::string& name, float value);
};

