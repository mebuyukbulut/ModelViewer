#pragma once
#include "Model.h"
#include "ShaderManager.h"
#include <memory>

class Camera;
class Shader;
class Mesh; 

class Renderer
{
public:

	enum class ShaderType{
		Main,
		Background,
		Grid,
		Light,
		Selection,

	};
private:

	ShaderManager _shaderManager;
	Shader* _shader{};
	Shader* _bgShader{};
	Shader* _gridShader{};
	Shader* _lightShader{};
	Shader* _selectShader{};

	std::shared_ptr<Camera> _camera;

	Mesh* _bgMesh{};
	Mesh* _gridMesh{};
public:

	void init();
	void terminate();

	void beginFrame();
	void endFrame();

	void drawModel(Model* model, const glm::mat4& transform);
	void drawBackground();
	void drawGrid();

	void setCamera(std::shared_ptr<Camera> camera);
	void setShader(const std::string name, ShaderType shaderType);
	void enableWireframe();

	Shader& getShader() { return *_shader; }

	//void setShader(const std::string& name) { _shader = shader; }
};

