#include "Entity.h"
#include "Transform.h"


Entity::Entity()
{
    transform = std::make_unique<Transform>();
    transform->owner = this;
}

Entity::~Entity() {
    terminate();
}

void Entity::addComponent(std::unique_ptr<Component> component)
{
    if (component) {
		component->owner = this;
	    components.emplace_back(std::move(component));
    }
}

void Entity::onInspect()
{
    //if (light.get()) light->drawUI();
    //if (model.get()) model->drawUI();
}

