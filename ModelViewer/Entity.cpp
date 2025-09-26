#include "Entity.h"
#include <imgui.h>
#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


glm::mat4  Transform::update() {
    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
        glm::radians(_rotation.x),
        glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f),
        glm::radians(_rotation.y),
        glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f),
        glm::radians(_rotation.z),
        glm::vec3(0.0f, 0.0f, 1.0f));

    // Y * X * Z
    const glm::mat4 roationMatrix = transformY * transformX * transformZ;

    // translation * rotation * scale (also know as TRS matrix)
    return glm::translate(glm::mat4(1.0f), _position) *
        roationMatrix *
        glm::scale(glm::mat4(1.0f), _scale);
}

glm::vec3 Transform::getPosition() { return _position; }
glm::vec3 Transform::getRotation() { return _rotation; }
glm::vec3 Transform::getScale() { return _scale; }
void Transform::setPosition(const glm::vec3& position) { _position = position; _isDirty = true; }
void Transform::setRotation(const glm::vec3& rotation) { _rotation = rotation; _isDirty = true; }
void Transform::setScale(const glm::vec3& scale) { _scale = scale; _isDirty = true; }


void Transform::drawUI()
{
	ImGui::SeparatorText("Transform"); 
	ImGui::DragFloat3("Position:", &_position[0], 0.01);
	ImGui::DragFloat3("Rotation:", &_rotation[0], 0.01);
	ImGui::DragFloat3("Scale:", &_scale[0], 0.01);
}

void Entity::draw() {
	_renderer->drawModel(model);

	for (auto& i : children)
		i->draw();
}