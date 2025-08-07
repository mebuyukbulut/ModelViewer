#pragma once
#include "Model.h"
#include "Shader.h"

class Renderer
{
	Shader _shader;
public:

	void init();
	void terminate();

	void beginFrame();
	void endFrame();

	void drawModel(Model& model);

	void enableWireframe();
};

