#pragma once
#include <memory>
#include "Entity.h"
#include "Renderer.h"
#include "Inspectable.h"

class SceneManager : public IInspectable
{
	std::list<std::shared_ptr<Transform>> _transforms;
    std::list <Transform*> _selectedTransforms{};
    Transform* _selectedTransform{};

    Renderer* _renderer;
    class Camera* _camera;
public:
	void init(Renderer* renderer, Camera* camera) {
        _renderer = renderer; 
        _camera = camera; 

        Transform* transform = new Transform;
        Entity* entity = new Entity;
        entity->model.reset(new Model);
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
};

