#include "ShaderManager.h"
#include "FileUtils.h"

void ShaderManager::init(){

	for(const auto& shaderInfo : shaderInfos) {
		Shader newShader;
		std::string vertexShaderSource = FileUtils::readFile(shaderInfo.vertexPath);
		std::string fragmentShaderSource = FileUtils::readFile(shaderInfo.fragmentPath);
		newShader.init(vertexShaderSource, fragmentShaderSource);
		shaders[shaderInfo.name] = newShader;
	}

}
void ShaderManager::terminate(){
	for(auto& pair : shaders)
		pair.second.terminate();
	shaders.clear();	
}

Shader& ShaderManager::getShader(const std::string& name){
	return shaders[name];
}
