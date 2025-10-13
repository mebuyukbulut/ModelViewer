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
#include "Camera.h"

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
    std::unique_ptr<MaterialManager> _materialMng{};
    RenderTarget _rt{};
public:
    void CreateRenderTarget(RenderTarget& rt, int width, int height) {
        rt.width = width;
        rt.height = height;

        glGenTextures(1, &rt.colorTex);
        glBindTexture(GL_TEXTURE_2D, rt.colorTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenRenderbuffers(1, &rt.depthRbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rt.depthRbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

        glGenFramebuffers(1, &rt.fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, rt.fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rt.colorTex, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rt.depthRbo);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            printf("RenderTarget incomplete!\n");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void ResizeRenderTarget(int newWidth, int newHeight)
    {
        if (newWidth == _rt.width && newHeight == _rt.height)
            return; // boyut değişmemiş

        _rt.width = newWidth;
        _rt.height = newHeight; 

        if (_rt.width && _rt.height)
            _camera->setWindowSize(_rt.width, _rt.height);

        // Eski GPU kaynaklarını serbest bırak
        glDeleteFramebuffers(1, &_rt.fbo);
        glDeleteTextures(1, &_rt.colorTex);
        glDeleteRenderbuffers(1, &_rt.depthRbo);

        // Yeni boyutla tekrar oluştur
        CreateRenderTarget(_rt, newWidth, newHeight);
    }


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

        
        CreateRenderTarget(_rt, 300, 300);

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
        // FBO’ya çiz
        glBindFramebuffer(GL_FRAMEBUFFER, _rt.fbo);
        glViewport(0, 0, _rt.width, _rt.height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        for (auto& transform : _transforms) {
            if(transform.get()->getEntity()->model)
                transform->draw(_renderer);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);


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

