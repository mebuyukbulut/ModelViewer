#include "Entity.h"
#include <imgui.h>
#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <format>

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

int GLOBAL_ID_COUNTER = 1;
Transform::Transform()
{
    ID = GLOBAL_ID_COUNTER++;
    GLOBAL_ID_COUNTER+=30;
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
    node["rotation"] = _rotation; 
    node["scale"] = _scale; 

    // light    
    if (_entity->light) node["light"].push_back(_entity->light->serialize());
    //node["light"].push_back(_entity->light ? _entity->light->serialize() : YAML::Node());
    // model 
    if (_entity->model) node["modelPath"] = (_entity->model->getPath());
    //node["modelPath"] = _entity->model ? _entity->model->getPath() : "";


    // serialize child of transform 
    //    // entity
    // components -> model, light, materials etc. 
    return node;
}

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
void Transform::drawAsColor(Renderer* renderer)
{
    renderer->drawModelAsColor(_entity->model.get(), getGlobalMatrix(), ID);

    for (auto& i : children)
        i->drawAsColor(renderer);
}




void Transform::terminate() {
    if (_entity)
    {
        _entity->terminate();
        delete _entity;
    }
}
void Transform::drawUI()
{
    ImGui::SeparatorText("Transform");
    bool p = ImGui::DragFloat3("Position:", &_position[0], 0.01);
    bool r = ImGui::DragFloat3("Rotation:", &_rotation[0], 0.01);
    bool s = ImGui::DragFloat3("Scale:", &_scale[0], 0.01);
    if (p || r || s) _isDirty = true;

    Light* light = _entity->light.get();
    if (p && light)
        light->position = _position;

    if (r && light) {
        SpotLight* spotlight = dynamic_cast<SpotLight*>(light);
        if (spotlight)
            spotlight->setDirection(_rotation) ;
    }

    _entity->drawUI();


}


void Entity::drawUI()
{
    if (light.get()) light->drawUI();
    if (model.get()) model->drawUI();
}



std::unique_ptr<Transform> TransformFactory::create(const YAML::Node& node, MaterialManager* materialManager)
{

    auto transform = std::make_unique<Transform>();
    transform->setEntity(new Entity);
    transform->name = node["name"].as<std::string>();
    transform->setPosition(node["position"].as<glm::vec3>());
    transform->setRotation(node["rotation"].as<glm::vec3>());
    transform->setScale(node["scale"].as<glm::vec3>());

    if (node["light"].IsDefined()) {
        for(const auto& light : node["light"])
            transform->getEntity()->light = std::move(LightFactory::create(light));
        transform->getEntity()->light->position = transform->getPosition();
    }

    if (node["modelPath"].IsDefined()) {
        std::string path = node["modelPath"].as<std::string>();
        Model* model = new Model(materialManager);
        model->loadFromFile(path);
        transform->getEntity()->model.reset(model);
        LOG_TRACE(std::format("Path:{}", path));

    }



    //if (auto lightNode = node["light"]; lightNode && lightNode.IsDefined()) {
    //    LOG_INFO("Light was found");
    //}
    //else {
    //    LOG_ERROR("Light was not found");
    //}

    return transform;
}