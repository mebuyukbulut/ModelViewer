#pragma once
#include <memory>
#include <list>
#include <glm/glm.hpp>

#include "Scene.h"
#include "Entity.h"
#include "Material.h"

class Renderer;

struct RenderTarget {
    GLuint fbo;
    GLuint colorTex;
    GLuint depthRbo;
    int width, height;
};

class SceneManager : public IInspectable
{
    Scene scene;

	std::list<std::shared_ptr<Transform>> _transforms{};
    std::list <Transform*> _selectedTransforms{};
    Transform* _selectedTransform{};

    Renderer* _renderer{};
    class Camera* _camera{};
    class UIManager* _UI{};
    std::unique_ptr<MaterialManager> _materialMng{};
    RenderTarget _rt{};

    bool isScenePopupOpen = false;
    bool isSelect = false; 
    glm::vec2 mousePos{};
    glm::vec2 viewportPos; // ImVec2
    glm::vec2 viewportPanelSize; // ImVec2
public:
    void CreateRenderTarget(RenderTarget& rt, int width, int height);
    void ResizeRenderTarget(int newWidth, int newHeight);


    void init(Renderer* renderer, Camera* camera, Shader* shader, UIManager* UI);
    void draw();
    
    Transform* getSelectedTransform() {
        //if (_selectedTransforms.empty()) return nullptr; 
        return _selectedTransform;
    }

    void loadScene(std::string path);
    void saveScene();

    // Inherited via IInspectable
    void drawUI() override;
    void drawGizmo();

    // LIGHTS
    void addLight(LightType lightType);
    void addLight(std::unique_ptr<Light> light);
    void configShader(Shader& shader);

    // SHAPES
    void addShape(DefaultShapes shape); 
    
    void deleteSelected();
    std::string getUniqueName(std::string name);
    bool isUniqueName(std::string name);

};

