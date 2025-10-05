#pragma once
#include <memory>
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <fstream>


#include "Entity.h"
#include "Renderer.h"
#include "Inspectable.h"
#include "LightManager.h"
#include "EventDispatcher.h"
#include "Material.h"
#include "Shader.h"
#include "Scene.h"


class SceneManager : public IInspectable
{
    Scene scene; 

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

        _materialMng.reset(new MaterialManager(shader));


        //Transform* transform = new Transform;
        //Entity* entity = new Entity;
        //_materialMng.reset(new MaterialManager(shader));
        //entity->model.reset(new Model(_materialMng.get()));
        //entity->model->loadFromFile("models\\monkey.obj");
        //entity->_renderer = renderer;
        //transform->setEntity(entity);
        //transform->name = "myobject"; 
        //_transforms.emplace_back(transform);

        //Transform* t2 = new Transform;
        //t2->name = "my new object";
        //t2->setEntity(entity);
        //_transforms.emplace_back(t2);
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

    void loadScene(std::string path) {
        LOG_TRACE("Load Scene");

        //YAML::Node root = YAML::LoadFile(path);

        //for (const auto& lightNode : root["Lights"]) {
        //    auto light = LightFactory::create(lightNode);

        //    if (light.get()) {

        //        LOG_TRACE("Load light");
        //        addLight(std::move(light));
        //    }
        //}


        YAML::Node rootTest = YAML::LoadFile("save0.yaml");
        for (const auto& transformNode : rootTest) {
            auto transform = TransformFactory::create(transformNode, _materialMng.get());

            if (transform.get()) {
                
                LOG_TRACE("Load transform...");
                _transforms.emplace_back(std::move(transform));
                //addLight(std::move(transform));
            }
        }
    }
    void saveScene() {
        LOG_TRACE("Save Scene");
        YAML::Node node;
        for (auto transform : _transforms) {
            node.push_back(transform->serialize());
            //Light* light = transform->getEntity()->light.get();
            //if(light)
            //    node["Lights"].push_back(light->serialize());
        }
        std::string path = "save0.yaml"; 
        std::ofstream fout(path);
        fout << node;
    }

    // Inherited via IInspectable
    void drawUI() override;

    void drawGizmo();

    // LIGHTS
    void addLight(LightType lightType);
    void addLight(std::unique_ptr<Light> light);
    void configShader(Shader& shader);
};

