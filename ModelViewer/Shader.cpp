#include "Shader.h"

#include <glad/glad.h>
#include <iostream>

std::string vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";


std::string fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

// compile shader code
unsigned int Shader::compileShader(std::string shaderCode, unsigned int shaderType)
{
    //std::cout << "Compiling shader of type: " 
	//	<< ((shaderType == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT") << std::endl;
	//std::cout << "Shader code:\n" << shaderCode << std::endl << std::endl;


	const char* shaderSource = shaderCode.c_str();
    unsigned int newShader = glCreateShader(shaderType);
    glShaderSource(newShader, 1, &shaderSource, NULL);
    glCompileShader(newShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(newShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(newShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::"
            << ((shaderType == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT") <<
            "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

	return newShader;
}

// link vertex and fragment shader into a shader program
unsigned int Shader::linkProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

void Shader::init()
{
    unsigned int vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    _shaderProgram = linkProgram(vertexShader, fragmentShader);
}

void Shader::useShader(){
    glUseProgram(_shaderProgram);
}

void Shader::deleteShader(){
    glDeleteProgram(_shaderProgram);
}
