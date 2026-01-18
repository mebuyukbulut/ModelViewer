#include "Transform.h"
#include <imgui.h>
#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <format>
#include "Entity.h"


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
    //const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
    //    glm::radians(_eulerRotation.x),
    //    glm::vec3(1.0f, 0.0f, 0.0f));
    //const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f),
    //    glm::radians(_eulerRotation.y),
    //    glm::vec3(0.0f, 1.0f, 0.0f));
    //const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f),
    //    glm::radians(_eulerRotation.z),
    //    glm::vec3(0.0f, 0.0f, 1.0f));

    // Quaternion'u doğrudan rotasyon matrisine çeviriyoruz
    glm::mat4 rotationMatrix = glm::mat4_cast(_orientation);

    //// Y * X * Z
    //const glm::mat4 roationMatrix = transformY * transformX * transformZ;

    // translation * rotation * scale (also know as TRS matrix)
    return glm::translate(glm::mat4(1.0f), _position) *
        rotationMatrix *
        glm::scale(glm::mat4(1.0f), _scale);

}

// it will works parents -> children
// bu if we call child before parent it will not work properly 
glm::mat4 Transform::getGlobalMatrix() {
    return _isDirty ? update(), _globalModelMatrix : _globalModelMatrix;
}


glm::vec3 Transform::getPosition() { return _position; }
glm::vec3 Transform::getRotation() { return _eulerRotation; }
glm::vec3 Transform::getScale() { return _scale; }
void Transform::setPosition(const glm::vec3& position) { _position = position; _isDirty = true; }
void Transform::setRotation(const glm::vec3& eulerDegrees) {
    _eulerRotation = eulerDegrees;
    // Euler açılarını (derece) radyana çevirip Quaternion'a aktarıyoruz
    _orientation = glm::quat(glm::radians(eulerDegrees));
    _isDirty = true;
}
void Transform::setScale(const glm::vec3& scale) { _scale = scale; _isDirty = true; }


namespace YAML {
    template<>
    struct convert<glm::vec3> {
        // glm::vec3 → YAML (C++ objesini dosyaya yazarken)
        static Node encode(const glm::vec3& rhs) {
            Node node;
            //node.push_back(std::vector<float>{ rhs.x, rhs.y, rhs.z });
            node.SetStyle(YAML::EmitterStyle::Flow);
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        // YAML → glm::vec3 (dosyadan okurken)
        static bool decode(const Node& node, glm::vec3& rhs) {
            if (!node.IsSequence() || node.size() != 3)
                return false;
            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };
}


YAML::Node Transform::serialize() {
    YAML::Node node;

    node["name"] = name;
    node["position"] = _position;
    node["rotation"] = _eulerRotation;
    node["scale"] = _scale;

    // light    
    if (owner->getComponent<Light>()) node["light"].push_back(owner->getComponent<Light>()->serialize());
    //node["light"].push_back(_entity->light ? _entity->light->serialize() : YAML::Node());
    // model 
    if (owner->getComponent<Model>()) node["modelPath"] = (owner->getComponent<Model>()->getPath());
    //node["modelPath"] = _entity->model ? _entity->model->getPath() : "";


    // serialize child of transform 
    //    // entity
    // components -> model, light, materials etc. 
    return node;
}


void Transform::addChild(Transform* child)
{
    child->parent = this;
    children.push_back(child);
}

void Transform::onInspect()
{
    ImGui::SeparatorText("Transform");
    bool p = ImGui::DragFloat3("Position:", &_position[0], 0.01);
    bool r = ImGui::DragFloat3("Rotation:", &_eulerRotation[0], 0.01);
    bool s = ImGui::DragFloat3("Scale:", &_scale[0], 0.01);
    if (p || r || s) _isDirty = true;

    //Light* light = owner->getComponent<Light>();
    //if (p && light)
    //    light->position = _position;

    //if (r && light) {
    //    SpotLight* spotlight = dynamic_cast<SpotLight*>(light);
    //    if (spotlight)
    //        spotlight->setDirection(_rotation);
    //}

    //owner->onInspect();
}



std::unique_ptr<Entity> TransformFactory::create(const YAML::Node& node, MaterialManager* materialManager)
{
	auto entity = std::make_unique<Entity>();
    //auto entity1 = std::unique_ptr<Entity>();

    entity->transform->name = node["name"].as<std::string>();
    entity->transform->setPosition(node["position"].as<glm::vec3>());
    entity->transform->setRotation(node["rotation"].as<glm::vec3>());
    entity->transform->setScale(node["scale"].as<glm::vec3>());

    //if (node["light"].IsDefined()) {
    //    for (const auto& light : node["light"])
    //        transform->getEntity()->light = std::move(LightFactory::create(light));
    //    transform->getEntity()->light->position = transform->getPosition();
    //}

    //if (node["modelPath"].IsDefined()) {
    //    std::string path = node["modelPath"].as<std::string>();
    //    Model* model = new Model(materialManager);
    //    model->loadFromFile(path);
    //    transform->getEntity()->model.reset(model);
    //    LOG_TRACE(std::format("Path:{}", path));

    //}



    //if (auto lightNode = node["light"]; lightNode && lightNode.IsDefined()) {
    //    LOG_INFO("Light was found");
    //}
    //else {
    //    LOG_ERROR("Light was not found");
    //}

    return entity;
}
