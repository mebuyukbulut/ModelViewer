#pragma once
#include "Model.h"
#include "Entity.h"
#include "ShaderManager.h"
#include "RenderItem.h"
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

	ShaderManager _shaderManager; // burada olması gerçekten gerekli mi? 

	Shader* _materialShader{};
	Shader* _matcapShader{};
	Shader* _wireframeShader{};
	Shader* _backgroundShader{};
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

	
	void setupMaterialPass();
	void setupMatcapPass();
	void setupWireframePass();
	void setupBackgroundPass();
	void setupGridPass();
	void setupLightPass();
	void setupSelectionPass();

public:
	void materialPass(const std::vector<RenderItem>& renderItems);
	void matcapPass(const std::vector<RenderItem>& renderItems);
	void wireframePass(const std::vector<RenderItem>& renderItems);
	void backgroundPass();
	void gridPass();
	void lightPass();
	void selectionPass(const std::vector<RenderItem>& renderItems);

	
	void init(std::shared_ptr<Camera> camera);
	void terminate();

	void beginFrame();
	void endFrame();

	void bindViewportFBO() {
		glBindFramebuffer(GL_FRAMEBUFFER, _rt.fbo);
		glViewport(0, 0, _rt.width, _rt.height);
	}
	void bindDefaultFBO() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
	void clearBuffer() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

	void resizeViewport(int width, int height) { resizeRenderTarget(width, height); }
	GLuint getViewportImage() { return _rt.colorTex; }
	
	void drawRecursive(Entity* entity, bool isMaterialPass);
	void drawModelAsMaterial(Model* model, const glm::mat4& transform);
	void drawModelAsMatcap(Model* model, const glm::mat4& transform);
	void drawModelAsColor(Model* model, const glm::mat4& transform, uint32_t ID);
	
	uint32_t getSelection(glm::vec2 mousePos);

	void setViewMode(ViewMode mode);
	ViewMode getViewMode();

	void setCamera(std::shared_ptr<Camera> camera);

	//void setShader(const std::string& name) { _shader = shader; }
};

