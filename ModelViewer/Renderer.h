#pragma once
#include "Model.h"
#include "ShaderManager.h"
#include <memory>

class Camera;
class Shader;

class Renderer
{
public:

	enum class ShaderType{
		Main,
		Light,
		Selection
	};
private:

	ShaderManager _shaderManager;
	Shader* _shader{};
	Shader* _lightShader{};
	Shader* _selectShader{};

	std::shared_ptr<Camera> _camera;
public:

	void init();
	void terminate();

	void beginFrame();
	void endFrame();

	void drawModel(Model* model, const glm::mat4& transform);

	void setCamera(std::shared_ptr<Camera> camera);
	void setShader(const std::string name, ShaderType shaderType);
	void enableWireframe();

	Shader& getShader() { return *_shader; }

	//void setShader(const std::string& name) { _shader = shader; }
};

