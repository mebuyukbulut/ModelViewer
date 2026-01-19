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
public:

	std::unique_ptr <class Transform> transform{};
	std::vector<std::unique_ptr<Component>> components{};

	//std::unique_ptr <Model> model{};
	//std::unique_ptr <Light> light{};
	//std::vector<MaterialHandle> materials{};

	Entity();
	~Entity();

	void addComponent(std::unique_ptr <Component> component);

	template<typename T>
	T* getComponent();


	// Inherited via Object
	void onInspect() override;
	void serialize(YAML::Emitter& out) const override;
	void deserialize(const YAML::Node& node) override;

private:
	void terminate() {
		//if (model)
		//	model.get()->terminate();
	}

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
