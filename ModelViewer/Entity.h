#pragma once
#include <glm/glm.hpp>
#include <list>
#include <memory>

#include "Inspectable.h"
#include "Model.h"

class Transform : public IInspectable{
	bool _isDirty{ true };

	glm::vec3 _position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 _rotation{ 0.0f, 0.0f, 0.0f };
	glm::vec3 _scale{ 1.0f, 1.0f, 1.0f };

	//Global space information concatenate in matrix
	glm::mat4 _modelMatrix = glm::mat4(1.0f);

	void update();
public: 

	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();
	void setPosition(const glm::vec3& position);
	void setRotation(const glm::vec3& rotation);
	void setScale(const glm::vec3& scale);

	glm::mat4 getModelMatrix();

	// draw gizmo ? 

	// Inherited via IInspectable
	void drawUI() override;

};

class Renderer; 

class Entity : public IInspectable
{
public:
	Renderer* _renderer; 

	Transform transform; 
	std::unique_ptr <Model> model;
	std::string name; 

	std::list<std::unique_ptr<Entity>> children; 
	Entity* parent; 

	void draw();

	// Inherited via IInspectable
	void drawUI() override;
};

