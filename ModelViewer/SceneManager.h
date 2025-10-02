#pragma once
#include <memory>
#include "Entity.h"
#include "Renderer.h"
#include "Inspectable.h"
#include "LightManager.h"
#include "EventDispatcher.h"
#include "Material.h"
#include "Shader.h"

class SceneManager : public IInspectable
{
	std::list<std::shared_ptr<Transform>> _transforms{};
    std::list <Transform*> _selectedTransforms{};
    Transform* _selectedTransform{};

    Renderer* _renderer{};
    class Camera* _camera{};
    std::unique_ptr<MaterialManager> _materialMng{};

public:
	void init(Renderer* renderer, Camera* camera, Shader* shader) {
        _renderer = renderer; 
        _camera = camera; 


        dispatcher.subscribe(EventType::AddPointLight, [&](const Event& e) {
            addLight(LightType::Point);
            });
        dispatcher.subscribe(EventType::AddSpotLight, [&](const Event& e) {
            addLight(LightType::Spot);
            });
        dispatcher.subscribe(EventType::AddDirectionalLight, [&](const Event& e) {
            addLight(LightType::Directional);
            });



        Transform* transform = new Transform;
        Entity* entity = new Entity;
        _materialMng.reset(new MaterialManager(shader));
        entity->model.reset(new Model(_materialMng.get()));
        entity->model->loadFromFile("models\\monkey.obj");
        entity->_renderer = renderer;
        transform->setEntity(entity);
        transform->name = "myobject"; 
        _transforms.emplace_back(transform);

        Transform* t2 = new Transform;
        t2->name = "my new object";
        t2->setEntity(entity);
        _transforms.emplace_back(t2);
	}

    void draw() {
        for (auto& transform : _transforms) {
            if(transform.get()->getEntity()->model)
                transform->draw(_renderer);
        }
    }
    
    Transform* getSelectedTransform() {
        //if (_selectedTransforms.empty()) return nullptr; 
        return _selectedTransform;
    }



    // Inherited via IInspectable
    void drawUI() override;

    void drawGizmo();

    // LIGHTS
    void addLight(LightType lightType);
    void configShader(Shader& shader);
};

