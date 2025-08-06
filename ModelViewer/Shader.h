#pragma once
#include <string>

class Shader
{
	unsigned int _shaderProgram;

	unsigned int compileShader(std::string shaderCode, unsigned int shaderType);
	unsigned int linkProgram(unsigned int vertexShader, unsigned int fragmentShader);

public:
	void init();
	void useShader(); // use() ? 
	void deleteShader(); // destroy() ? 
};

