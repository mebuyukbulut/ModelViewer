#pragma once
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

#include <list>
#include <memory>

#include "Inspectable.h"
#include "Model.h"
#include "LightManager.h"
#include "Material.h"

class Transform : public IInspectable{
	std::list<std::unique_ptr<Transform>> children;
	Transform* parent{};
	class Entity* _entity{}; // Maybe we should use unique_ptr instead of ptr 
	bool _isSelected{ false };
	bool _isDirty{ true };

	glm::vec3 _position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 _rotation{ 0.0f, 0.0f, 0.0f };
	glm::vec3 _scale{ 1.0f, 1.0f, 1.0f };

	//Global space information concatenate in matrix
	glm::mat4 _globalModelMatrix = glm::mat4(1.0f);
	glm::mat4 _localModelMatrix = glm::mat4(1.0f);

	void update();
	glm::mat4 getLocalMatrix();

public: 
	uint32_t ID;
	std::string name{};

	Transform();

	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();
	void setPosition(const glm::vec3& position);
	void setRotation(const glm::vec3& rotation);
	void setScale(const glm::vec3& scale);

	YAML::Node serialize();

	glm::mat4 getGlobalMatrix();

	void addChild(std::unique_ptr<Transform> child); 
	void setEntity(Entity* entity);
	Entity* getEntity(); 

	void draw(class Renderer* renderer);
	void drawAsColor(class Renderer* renderer);

	bool isSelected() { return _isSelected; }
	void select() {
		_isSelected = true;

	}
	void deselect() {
		_isSelected = false;
	}
	
	void terminate();
	// draw gizmo ? 

	// Inherited via IInspectable
	void drawUI() override;

};

class Renderer; 

class Entity : public IInspectable
{
public:
	Renderer* _renderer{};
	Transform* transform{};

	std::unique_ptr <Model> model{};
	std::unique_ptr <Light> light{};
	//std::vector<MaterialHandle> materials{};

	void terminate() {
		if (model)
			model.get()->terminate();
	}

	// Inherited via IInspectable
	void drawUI() override;
};

class TransformFactory {
public:
	static std::unique_ptr<Transform> create(const YAML::Node& node, MaterialManager* materialManager);
};