#pragma once
#include <glm/glm.hpp>
#include <list>
#include <memory>

#include "Inspectable.h"
#include "Model.h"

class Transform : public IInspectable{
	std::list<std::unique_ptr<Transform>> children;
	Transform* parent{};
	class Entity* _entity{};
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
	std::string name{};
	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();
	void setPosition(const glm::vec3& position);
	void setRotation(const glm::vec3& rotation);
	void setScale(const glm::vec3& scale);


	glm::mat4 getGlobalMatrix();

	void addChild(std::unique_ptr<Transform> child); 
	void setEntity(Entity* entity);
	void draw(class Renderer* renderer);

	bool isSelected() { return _isSelected; }
	void select() {
		_isSelected = true;

	}
	void deselect() {
		_isSelected = false;
	}
	// draw gizmo ? 

	// Inherited via IInspectable
	void drawUI() override;

};

class Renderer; 

class Entity : public IInspectable
{
public:
	Renderer* _renderer; 
	Transform* transform; 

	std::unique_ptr <Model> model;

	// Inherited via IInspectable
	void drawUI() override;
};

