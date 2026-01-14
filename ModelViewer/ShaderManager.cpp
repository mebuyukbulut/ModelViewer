#include "ShaderManager.h"
#include "FileUtils.h"
#include "Logger.h"

void ShaderManager::init(){

	for(const auto& shaderInfo : shaderInfos) {
		Shader newShader;
		std::string vertexShaderSource = FileUtils::readFile(shaderInfo.vertexPath);
		std::string fragmentShaderSource = FileUtils::readFile(shaderInfo.fragmentPath);
		newShader.init(vertexShaderSource, fragmentShaderSource);
		newShader._type = shaderInfo.type;
		shaders[shaderInfo.name] = newShader;
	}

}

void ShaderManager::terminate(){
	for(auto& pair : shaders)
		pair.second.terminate();
	shaders.clear();	
}

Shader& ShaderManager::getShader(const std::string& name){
	if(!shaders.contains(name))
		LOG_CRITICAL("SHADER DOES NOT EXITS: "+name);
	return shaders[name];
}
