#pragma once
#include <vector>
#include <memory>

#include "Object.h"
#include "Component.h"



class Entity : public Object
{
	bool active{ true };
public:

	std::unique_ptr <class Transform> transform{};
	std::vector<std::unique_ptr<Component>> components{};

	Entity();
	~Entity();

	void setActive(bool active) { this->active = active; }
	bool isActive() const { return active; }

	void addComponent(std::unique_ptr <Component> component);
	template<typename T>
	T* getComponent();


	// Inherited via Object
	void onInspect() override;
	void serialize(YAML::Emitter& out) override;
	void deserialize(const YAML::Node& node) override;
};



template<typename T>
inline T* Entity::getComponent()
{
	for (auto& comp : components) {
		T* casted = dynamic_cast<T*>(comp.get());
		if (casted)
			return casted;
	}
	return nullptr;
}
