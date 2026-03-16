#pragma once
#include "Component.h"
#include <glm/gtc/quaternion.hpp>

#include "Model.h"
#include "LightManager.h"
#include "Material.h"

class Transform : public Component {
	static const bool registered;

	std::vector<Transform*> children;
	Transform* parent{};

	bool _isDirty{ true };

	glm::vec3 _position{ 0.0f, 0.0f, 0.0f };
	glm::quat _orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Başlangıç (Identity)
	glm::vec3 _eulerRotation{ 0.0f, 0.0f, 0.0f };
	glm::vec3 _scale{ 1.0f, 1.0f, 1.0f };

	//Global space information concatenate in matrix
	glm::mat4 _globalModelMatrix = glm::mat4(1.0f);
	glm::mat4 _localModelMatrix = glm::mat4(1.0f);

	void update();
	glm::mat4 getLocalMatrix();
	void decomposeMatrix(const glm::mat4& matrix);
public:
	Transform() {
		type = ComponentType::Transform;
	}

	glm::vec3 getGlobalPosition();

	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();

	void setPosition(const glm::vec3& position);
	void setRotation(const glm::vec3& eulerDegrees);
	void setOrientation(const glm::quat& orientation);
	void setScale(const glm::vec3& scale);



	void setLocalMatrix(const glm::mat4& worldMatrix);


	glm::mat4 getGlobalMatrix();

	void addChild(Transform* child);
	void removeChild(Transform* child);
	void setParent(Transform* newParent);

	std::vector<Transform*>& getChildren() { return children; }

	bool isRoot() { return parent == nullptr; }

	void onInspect() override;
	void serialize(YAML::Emitter& out) override;
	void deserialize(const YAML::Node& node) override;

};