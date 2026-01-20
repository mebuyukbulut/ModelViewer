#include "Entity.h"
#include "Transform.h"
#include "Logger.h"

#include "YAMLHelper.h"

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
    if (!transform) {
		LOG_ERROR("Entity has no transform!");
        return;
    }
    transform->onInspect();
    for(auto& component : components)
		component->onInspect();
}

void Entity::serialize(YAML::Emitter& out)
{
    out << YAML::BeginMap; 
	Object::serialize(out);

	out << YAML::Key << "transform" << YAML::Value;
	transform->serialize(out);

    out << YAML::Key << "components" << YAML::Value;
    out <<YAML::BeginSeq;
    for (const auto& component : components)
		component->serialize(out);
	out << YAML::EndSeq;

    out << YAML::EndMap;
}

void Entity::deserialize(const YAML::Node& node)
{
}

