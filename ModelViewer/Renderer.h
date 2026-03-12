#pragma once
#include "Model.h"
#include "ShaderManager.h"
#include <memory>

class Camera;
class Shader;
class Mesh; 
class Texture; 

enum class ViewMode {
	Wireframe,
	Matcap,
	Material,
};

struct RenderTarget {
	GLuint fbo;
	GLuint colorTex;
	GLuint depthRbo;
	int width, height;
};


class Renderer
{
public:

	enum class ShaderType{
		Main,
		Background,
		Grid,
		Light,
		Selection,
		Matcap,
		Material, 
	};
private:
	RenderTarget _rt{};

	ViewMode _viewMode{ ViewMode::Material };

	ShaderManager _shaderManager;
	Shader* _shader{};
	Shader* _materialShader{};
	Shader* _matcapShader{};

	Shader* _bgShader{};
	Shader* _gridShader{};

	Shader* _lightShader{};
	Shader* _selectionShader{};

	std::shared_ptr<Camera> _camera;

	Mesh* _bgMesh{};
	Mesh* _gridMesh{};
	Texture* cubemapTexture;
	//std::vector<Texture*> loadedMatcapTextures; 
	std::shared_ptr<Texture> matcapTexture;
	std::vector<std::string> matcapTexturePaths;
	void initMatcap();
	//void initSkybox();

	void createRenderTarget(RenderTarget& rt, int width, int height);
	void resizeRenderTarget(int newWidth, int newHeight);

public:
	
	void init(std::shared_ptr<Camera> camera);
	void terminate();

	void beginFrame();
	void endFrame();

	void bindViewportFBO() {
		glBindFramebuffer(GL_FRAMEBUFFER, _rt.fbo);
		glViewport(0, 0, _rt.width, _rt.height);
	}
	void bindDefaultFBO() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	void resizeViewport(int width, int height) { resizeRenderTarget(width, height); }
	GLuint getViewportImage() { return _rt.colorTex; }

	void drawModel(Model* model, const glm::mat4& transform);
	void drawModelAsColor(Model* model, const glm::mat4& transform, uint32_t ID);
	
	void drawBackground();
	void drawGrid();
	uint32_t getSelection(glm::vec2 mousePos);

	void setViewMode(ViewMode mode);
	ViewMode getViewMode();

	void setCamera(std::shared_ptr<Camera> camera);
	void setShader(const std::string name, ShaderType shaderType);
	//void enableWireframe();

	Shader& getShader() { return *_shader; }

	//void setShader(const std::string& name) { _shader = shader; }
};

