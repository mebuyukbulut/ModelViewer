#include "LightManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>

#include <iostream>
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Camera.h"

// I cannot find yet true formula for this conversation  
// for that reason I use that approximation
glm::vec3 kelvin2RGB_fast(float kelvin) {
    kelvin /= 100; 
    glm::vec3 color{ 0,0,0 };

    if (kelvin <= 66) {
        color.r = 255;

        color.g = kelvin;
        color.g = 99.4708025861 * std::log(color.g) - 161.1195681661;

        if (kelvin <= 19) {
            color.b = 0;
        }
        else {
            color.b = kelvin - 10;
            color.b = 138.5177312231 * std::log(color.b) - 305.0447927307;
        }
    }
    else {
        color.r = kelvin - 66;
        color.r = 329.698727446 * std::pow(color.r, -0.1332047592);

        color.g = kelvin - 60;
        color.g = 288.1221695283 * std::pow(color.g, -0.0755148492);

        color.b = 255;
    }

    return  glm::clamp(color / 255.f, glm::vec3(0), glm::vec3(1));
}

void LightManager::drawUI() {
    // Shader selection combo box
    const char* items[] = { "Point", "Direction", "Spot" };
    static int currentItem = 0;

    ImGui::Begin("Light Controls"); 

    ImGui::SeparatorText("Add Light");
    ImGui::Combo("type", &currentItem, items, IM_ARRAYSIZE(items));
	if(ImGui::Button("Add Light"))
        addLight(currentItem);


    ImGui::SeparatorText("Light List");
	
    if (ImGui::BeginListBox("listbox 1"))
    {
        for (int i{}; i < _lights.size(); i++) //(int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
			std::string lightName = _lights[i]->name;
            const bool is_selected = (selectedLight == i);
            if (ImGui::Selectable(lightName.c_str(), is_selected)) {
				//std::cout << "Selected light: " << light.name << std::endl;
                selectedLight = i;
            }
            //if (ImGui::IsItemHovered())
            //    std::cout << "hovered light: " << light.name << std::endl;
            //// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            //if (is_selected)
            //    ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }


    ImGui::SeparatorText("Light properties");
	if (selectedLight == -1) { ImGui::End(); return; } // No light selected

    _lights[selectedLight]->drawUI();

    ImGui::End();
}

void LightManager::drawGizmo()
{
	if (selectedLight == -1) return; // No light selected


    // Make sure to call inside ImGui frame:
    ImGuizmo::BeginFrame();

    // Get viewport size
    ImVec2 windowPos(0, 0);// = ImGui::GetWindowPos();
   
	auto windowSize = _camera->getWindowSize();

    // Setup ImGuizmo rect
    ImGuizmo::SetRect(windowPos.x, windowPos.y, windowSize.x, windowSize.y);

    // Choose operation: translate / rotate / scale
    static ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;

    glm::mat4 modelMatrix(1);
	modelMatrix = glm::translate(glm::mat4(1.0f), _lights[selectedLight]->position);

    ImGuizmo::Manipulate(
        glm::value_ptr(_camera->getViewMatrix()),
        glm::value_ptr(_camera->getProjectionMatrix()),
        mCurrentGizmoOperation,
        ImGuizmo::LOCAL,
        glm::value_ptr(modelMatrix)
    );


    // If modified, write back into your glm::mat4
    if (ImGuizmo::IsUsing()) {
		_lights[selectedLight]->position = glm::vec3(modelMatrix[3]); 
    }
}

void LightManager::configShader(Shader& shader)
{
    for (int i{}; i < _lights.size(); i++) {
		std::string prefix = "pointLights[" + std::to_string(i) + "].";
        _lights[i]->configShader(shader, prefix);
    }
	shader.setInt("numLights", static_cast<int>(_lights.size()));

}
void LightManager::addLight(int lightType)
{
    static int lightCounter = 0; // Static counter to keep track of light names

    if (lightType < 0 || lightType > 2) {
		std::cout << "ERROR: light type must be 0, 1 or 2!" << std::endl;
		return; // Invalid light type
    }
    if (_lights.size() >= MAX_LIGHTS) {
		std::cout << "ERROR: Maximum number of lights reached!" << std::endl;
		return; // Prevent adding more lights if the limit is reached
    }
    std::unique_ptr<Light> newLight; 

    if (lightType == 0) { // Point 
        newLight = std::make_unique<PointLight>(); 
        newLight->type = 0; 
    } 
    else if (lightType == 1) { // Spot 
        newLight = std::make_unique<SpotLight>(); 
        newLight->type = 0; 
    }
    else if (lightType == 2) { // Directional 
        newLight = std::make_unique<DirectionalLight>();
        newLight->type = 0;
    }

	_lights.push_back(std::move(newLight));
}

void Light::configShader(Shader& shader, std::string prefix) {
    shader.setVec3(prefix + "position", position);
    shader.setVec3(prefix + "color", color);
    shader.setFloat(prefix + "intensity", intensity);
    shader.setInt(prefix + "type", type);
}
void PointLight::configShader(Shader& shader, std::string prefix)
{
    Light::configShader(shader, prefix);
    shader.setFloat(prefix + "attenuation", attenuation);
}
void SpotLight::configShader(Shader& shader, std::string prefix)
{
    Light::configShader(shader, prefix);
    shader.setVec3(prefix + "direction", direction);
    shader.setFloat(prefix + "cutoff", cutoff);
    shader.setFloat(prefix + "attenuation", attenuation);
}
void DirectionalLight::configShader(Shader& shader, std::string prefix)
{
    Light::configShader(shader, prefix);
    shader.setVec3(prefix + "direction", direction);
}
void Light::drawUI()
{
    ImGui::DragFloat3("Position", &position[0], 0.1f);
    ImGui::ColorEdit3("Color", &color[0]);
    static bool kelvinCB = false;
    static float kelvin{ 1000.0f };
    ImGui::Checkbox("Use temperature", &kelvinCB);
    if (kelvinCB) {
        ImGui::DragFloat("Kelvin", &kelvin, 10.0f, 1000.0f, 15'000.0f);
        color = kelvin2RGB_fast(kelvin);
    }

    ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.0f, 100.0f);
}
void PointLight::drawUI()
{
    Light::drawUI();
    ImGui::DragFloat("Attenuation", &attenuation, 0.1f, 0.1f, FLT_MAX);
}
void SpotLight::drawUI()
{
    Light::drawUI();
    ImGui::DragFloat3("Direction", &direction[0], 0.1f);
    ImGui::DragFloat("Cutoff", &cutoff, 1.0f, 0.0f, 90.0f);
    ImGui::DragFloat("Attenuation", &attenuation, 0.1f, 0.1f, FLT_MAX);
}
void DirectionalLight::drawUI()
{
    Light::drawUI();
    ImGui::DragFloat3("Direction", &direction[0], 0.1f);
}
