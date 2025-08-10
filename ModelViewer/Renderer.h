#pragma once
#include "Model.h"
#include "ShaderManager.h"

class Camera;
class Shader;

class Renderer
{
	ShaderManager _shaderManager;
	Shader* _shader{};
	Camera* _camera;
public:

	void init();
	void terminate();

	void beginFrame();
	void endFrame();

	void drawModel(Model& model);

	void setCamera(Camera* camera) { _camera = camera; }

	void enableWireframe();

	Shader& getShader() { return *_shader; }
	//void setShader(const std::string& name) { _shader = shader; }
};

