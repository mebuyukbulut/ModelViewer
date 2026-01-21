#pragma once
#include <vector>
#include <memory>

//#include <glm/glm.hpp>
//#include <list>

#include "Object.h"
#include "Component.h"
//#include "Inspectable.h"



class Entity : public Object
{
	bool active{ true };
	class MaterialManager* _materialManager{}; // bunu eklemeden bir yol bulmaliyiz
public:
	void setMaterialManager(MaterialManager* materialManager);

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

private:
	//void terminate() {
	//	//if (model)
	//	//	model.get()->terminate();
	//}

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
