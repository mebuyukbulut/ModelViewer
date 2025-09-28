#pragma once
#include <memory>
#include "Entity.h"
#include "Renderer.h"

class SceneManager
{
	std::list<std::shared_ptr<Transform>> _transforms;
	Transform* selected_transform{};
    Renderer* _renderer;
public:
	void init(Renderer* renderer) {
        _renderer = renderer; 

        Transform* transform = new Transform;
        Entity* entity = new Entity;
        entity->model.reset(new Model);
        entity->model->loadFromFile("models\\monkey.obj");
        entity->_renderer = renderer;
        transform->setEntity(entity);
        _transforms.emplace_back(transform);
	}

    void draw() {
        for (auto& transform : _transforms) {
            transform->draw(_renderer);
        }
    }
    
    Transform* getSelectedTransform() {
        selected_transform = _transforms.front().get();
        return selected_transform;
    }


};

