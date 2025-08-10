#pragma once
#include "Model.h"
#include "Shader.h"

class Camera;

class Renderer
{
	Shader _shader;
	Camera* _camera;
public:

	void init();
	void terminate();

	void beginFrame();
	void endFrame();

	void drawModel(Model& model);

	void setCamera(Camera* camera) { _camera = camera; }

	void enableWireframe();

	Shader& getShader() { return _shader; }
};

