#pragma once
#include "Model.h"
#include "Shader.h"

class Renderer
{
public:
	Shader _shader;

	void init();
	void terminate();

	void beginFrame();
	void endFrame();

	void drawModel(Model& model);

	void enableWireframe();
};

