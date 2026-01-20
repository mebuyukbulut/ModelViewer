#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "Scene.h"
#include "Entity.h"
#include "Transform.h"
#include "Material.h"

class Renderer;

struct RenderTarget {
    GLuint fbo;
    GLuint colorTex;
    GLuint depthRbo;
    int width, height;
};

class SceneManager : public Object
{
    Scene scene;

	std::vector<std::unique_ptr<Entity>> _entities{};
    std::vector <Entity*> _selectedEntities{};
    Entity* _selectedEntity{};
	std::vector<std::unique_ptr<Entity>> _pendingEntities{};

    Renderer* _renderer{};
    class Camera* _camera{};
    class UIManager* _UI{};

    std::unique_ptr<class LightManager> _lightMng{};
    std::unique_ptr<MaterialManager> _materialMng{};

    RenderTarget _rt{};

    bool isScenePopupOpen = false;
    bool isViewportSelect = false; 
    glm::vec2 mousePos{};
    glm::vec2 viewportPos; // ImVec2
    glm::vec2 viewportPanelSize; // ImVec2

public:
	SceneManager() = default;
    ~SceneManager() = default; 

    void CreateRenderTarget(RenderTarget& rt, int width, int height);
    void ResizeRenderTarget(int newWidth, int newHeight);

    void fileLoadManager();
    void init(Renderer* renderer, Camera* camera, Shader* shader, UIManager* UI);
    void draw();
    void drawRecursive(Entity* entity);
    void drawAsColorRecursive(Entity* entity);

    Entity* getSelectedEntity() {
        //if (_selectedTransforms.empty()) return nullptr; 
        return _selectedEntity;
    }

    bool isSelected(Entity* entity);
    bool isLastSelected(Entity* entity);

    void select(Entity* entity);
    void deselect(Entity* entity);
    void deselectAll();


    void loadScene(std::string path);
    void saveScene();

    void drawHierarchyTreeRecursive(Entity* entity);
	void onInspect() override;
    void drawGizmo();

    // LIGHTS
    void addLight(LightType lightType);
    void sceneQuery(); // (Shader& shader);

    // SHAPES
    void addShape(DefaultShapes shape); 
    void addMonkey();
    
    void deleteSelected();
    std::string getUniqueName(std::string name);
    bool isUniqueName(std::string name);


    // Inherited via Object
    void serialize(YAML::Emitter& out) override;
    void deserialize(const YAML::Node& node) override;

};

