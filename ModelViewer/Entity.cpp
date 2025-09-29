#include "Entity.h"
#include <imgui.h>
#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Transform::update() 
{
    if (_isDirty) {
        _localModelMatrix = getLocalMatrix();
        _isDirty = false;
    }
    _globalModelMatrix = parent ? parent->_globalModelMatrix * _localModelMatrix : _localModelMatrix;

    for (auto&& child : children)
        child->update();

}

glm::mat4 Transform::getLocalMatrix()
{
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
// it will works parents -> children
// bu if we call child before parent it will not work properly 
glm::mat4 Transform::getGlobalMatrix() { 
    return _isDirty ? update(), _globalModelMatrix : _globalModelMatrix;
}

glm::vec3 Transform::getPosition() { return _position; }
glm::vec3 Transform::getRotation() { return _rotation; }
glm::vec3 Transform::getScale() { return _scale; }
void Transform::setPosition(const glm::vec3& position) { 
    _position = position; 
    _isDirty = true; 

    if (Light* light = _entity->light.get()) 
        light->position = _position;
}
void Transform::setRotation(const glm::vec3& rotation) { _rotation = rotation; _isDirty = true; }
void Transform::setScale(const glm::vec3& scale) { _scale = scale; _isDirty = true; }



void Transform::addChild(std::unique_ptr<Transform> child)
{
    child->parent = this; 
    children.emplace_back(std::move(child));
}

void Transform::setEntity(Entity* entity) {
    _entity = entity;
    _entity->transform = this;
}

Entity* Transform::getEntity()
{
    return _entity;
}

void Transform::draw(Renderer* renderer)
{
    renderer->drawModel(_entity->model.get(), getGlobalMatrix());

    for (auto& i : children)
    	i->draw(renderer);
}

void Transform::drawUI()
{
    ImGui::SeparatorText("Transform");
    bool p = ImGui::DragFloat3("Position:", &_position[0], 0.01);
    bool r = ImGui::DragFloat3("Rotation:", &_rotation[0], 0.01);
    bool s = ImGui::DragFloat3("Scale:", &_scale[0], 0.01);
    if (p || r || s) _isDirty = true;

    if (Light* light = _entity->light.get()) {
        light->drawUI();
        if (p)
            light->position = _position;

    }
}



void Entity::drawUI()
{

}
