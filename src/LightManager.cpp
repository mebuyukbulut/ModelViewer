#include "LightManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "YAMLHelper.h"

#include "Camera.h"
#include "Entity.h"
#include "Transform.h"


const bool PointLight::registered = []() {
    ComponentFactory::registerType(ComponentType::PointLight, []() { return new PointLight(); });
    return true;
    }();
const bool SpotLight::registered = []() {
    ComponentFactory::registerType(ComponentType::SpotLight, []() { return new SpotLight(); });
    return true;
    }();
const bool DirectionalLight::registered = []() {
    ComponentFactory::registerType(ComponentType::DirectionalLight, []() { return new DirectionalLight(); });
    return true;
    }();



// I cannot find yet true formula for this conversation  
// for that reason I use that approximation
glm::vec3 kelvin2RGB_fast(float kelvin) {
    kelvin /= 100; 
    glm::vec3 color{ 0,0,0 };

    if (kelvin <= 66) {
        color.r = 255;

        color.g = kelvin;
        color.g = 99.4708025861f * std::log(color.g) - 161.1195681661f;

        if (kelvin <= 19) {
            color.b = 0;
        }
        else {
            color.b = kelvin - 10;
            color.b = 138.5177312231f * std::log(color.b) - 305.0447927307f;
        }
    }
    else {
        color.r = kelvin - 66;
        color.r = 329.698727446f * std::pow(color.r, -0.1332047592f);

        color.g = kelvin - 60;
        color.g = 288.1221695283f * std::pow(color.g, -0.0755148492f);

        color.b = 255;
    }

    return  glm::clamp(color / 255.f, glm::vec3(0), glm::vec3(1));
}


void Light::update()
{
    
    blockData.setPosition(owner->transform->getGlobalPosition());
	// update direction for spot and directional lights
    //isDirty = false; 
}

Light::Light() {
    name = "Light Component";

    blockData.setPosition(glm::vec3(0, 0, 0));
    blockData.setColor(glm::vec3(1, 1, 1));
    blockData.setIntensity(1);
    blockData.setType(LightType::None);
}

PointLight::PointLight() {
    name = "Point Light Component";
	type = ComponentType::PointLight;

    blockData.setType(LightType::Point);
    blockData.setAttenuation(3);
}

SpotLight::SpotLight() {
    name = "Spot Light";
	type = ComponentType::SpotLight;

    blockData.setType(LightType::Spot);
    blockData.setDirection(glm::vec3(0, -1, 0));
    blockData.setCutoff(25); 
    blockData.setAttenuation(3);
}

DirectionalLight::DirectionalLight() {
    name = "Directional Light Component";
	type = ComponentType::DirectionalLight;

    blockData.setType(LightType::Directional);    
    blockData.setDirection(glm::vec3(0, -1, 0));
}



void Light::serialize(YAML::Emitter& out)
{
    out << YAML::BeginMap;
    Component::serialize(out);
    out << YAML::Key << "light_type" << YAML::Value << static_cast<int>(blockData.getType());
    out << YAML::Key << "position" << YAML::Value << blockData.getPosition();
	out << YAML::Key << "color" << YAML::Value << blockData.getColor();
	out << YAML::Key << "intensity" << YAML::Value << blockData.getIntensity();
	out << YAML::Key << "attenuation" << YAML::Value << blockData.getAttenuation();
	out << YAML::Key << "cutoff" << YAML::Value << blockData.getCutoff();
	out << YAML::Key << "direction" << YAML::Value << blockData.getDirection();
	out << YAML::Key << "kelvinCB" << YAML::Value << kelvinCB;
	out << YAML::Key << "kelvin" << YAML::Value << kelvin;
    out << YAML::EndMap;
}

void Light::deserialize(const YAML::Node& node)
{
    Component::deserialize(node);
    blockData.setPosition(node["position"].as<glm::vec3>());
    blockData.setColor(node["color"].as<glm::vec3>());
    blockData.setIntensity(node["intensity"].as<float>());
    blockData.setAttenuation(node["attenuation"].as<float>());
    blockData.setCutoff(node["cutoff"].as<float>());
    blockData.setDirection(node["direction"].as<glm::vec3>());
    kelvinCB = node["kelvinCB"].as<bool>();
	kelvin = node["kelvin"].as<float>();
}

void Light::onInspect()
{
    //ImGui::DragFloat3("Position", &position[0], 0.1f);
    ImGui::ColorEdit3("Color", blockData.getColorPtr());
    ImGui::Checkbox("Use temperature", &kelvinCB);
    if (kelvinCB) {
        ImGui::DragFloat("Kelvin", &kelvin, 10.0f, 1000.0f, 15'000.0f);
        blockData.setColor(kelvin2RGB_fast(kelvin));
    }

    ImGui::DragFloat("Intensity", blockData.getIntensityPtr(), 0.1f, 0.0f, 100.0f);
}
void PointLight::onInspect()
{
    ImGui::SeparatorText("PointLight");
    Light::onInspect();
    ImGui::DragFloat("Attenuation", blockData.getAttenuationPtr(), 0.1f, 0.1f, FLT_MAX);
}
void SpotLight::onInspect()
{
    ImGui::SeparatorText("SpotLight");
    Light::onInspect();
    ImGui::DragFloat3("Direction", blockData.getDirectionPtr(), 0.1f);
    ImGui::DragFloat("Cutoff", blockData.getCutoffPtr(), 1.0f, 0.0f, 90.0f);
    ImGui::DragFloat("Attenuation", blockData.getAttenuationPtr(), 0.1f, 0.1f, FLT_MAX);
}
void DirectionalLight::onInspect()
{
    ImGui::SeparatorText("DirectionalLight");
    Light::onInspect();
    ImGui::DragFloat3("Direction", blockData.getDirectionPtr(), 0.1f);
}


void SpotLight::setDirection(glm::vec3 rotation)
{
    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
        glm::radians(rotation.x),
        glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f),
        glm::radians(rotation.y),
        glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f),
        glm::radians(rotation.z),
        glm::vec3(0.0f, 0.0f, 1.0f));
    // Y * X * Z
    const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

    blockData.setDirection(rotationMatrix * glm::vec4(0, 1, 0, 1));

}

//
//std::unique_ptr<Light> LightFactory::create(const YAML::Node& node)
//{
//    std::string typeStr = node["type"].as<std::string>();
//
//	GPULight gpulight;
//	gpulight.setPosition(node["position"].as<glm::vec3>());
//	gpulight.setColor(node["color"].as<glm::vec3>());
//    gpulight.setIntensity(node["intensity"].as<float>());    
//	gpulight.setAttenuation(node["attenuation"].as<float>());
//    gpulight.setCutoff(node["cutoff"].as<float>());
//	gpulight.setDirection(node["direction"].as<glm::vec3>());
//
//    if (typeStr == "point") {
//        gpulight.setType(LightType::Point);        
//        return std::make_unique<PointLight>(gpulight);
//    }
//    else if (typeStr == "spot") {
//        gpulight.setType(LightType::Spot);
//        return std::make_unique<SpotLight>(gpulight);
//    }
//    else if (typeStr == "directional") {
//        gpulight.setType(LightType::Directional);
//        return std::make_unique<DirectionalLight>(gpulight);
//    }
//
//    return nullptr;
//}

LightManager::LightManager()
{
	LOG_TRACE("LightManager created.");

    glGenBuffers(1, &uboLights);
    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(GPULightBlock), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboLights); // 0 numaralı binding point

}

LightManager::~LightManager()
{
	glDeleteBuffers(1, &uboLights);
}

void LightManager::queryLights(const std::vector<Light*> lights)
{
    blockData.numLights = static_cast<int>(lights.size());

    for (size_t i = 0; i < lights.size() && i < 8; i++) {
        lights[i]->update();
        blockData.lights[i] = lights[i]->getGPULight();
	}

    // activeLights verilerini GPULightBlock'a kopyala...

    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GPULightBlock), &blockData);

}

std::unique_ptr<Light> LightManager::createLight(LightType type)
{
	int lightCount = blockData.numLights;
    if (lightCount >= MAX_LIGHTS) {
		LOG_ERROR("Maximum number of lights reached!");
		return std::unique_ptr<Light>();
    }

    std::unique_ptr<Light> newLight;
    std::string lightName = "";
    switch (type)
    {
    case LightType::Directional:
        newLight = std::make_unique<DirectionalLight>();
        lightName = "Directional Light" + std::to_string(lightCount);
        break;
    case LightType::Point:
        newLight = std::make_unique<PointLight>();
        lightName = "Point Light" + std::to_string(lightCount);
        break;
    case LightType::Spot:
        newLight = std::make_unique<SpotLight>();
        lightName = "Spot Light" + std::to_string(lightCount);
        break;
    default:
        break;
    }


    return newLight;
}
