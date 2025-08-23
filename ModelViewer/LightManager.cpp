#include "LightManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Camera.h"

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
			Light& light = _lights[i];
            const bool is_selected = (selectedLight == i);
            if (ImGui::Selectable(light.name.c_str(), is_selected)) {
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
	ImGui::DragFloat3("Position", &_lights[selectedLight].position[0], 0.1f);
    ImGui::ColorEdit3("Color", &_lights[selectedLight].color[0]);
	ImGui::DragFloat("Intensity", &_lights[selectedLight].intensity, 0.1f, 0.0f, 100.0f);
	if (_lights[selectedLight].type != 0) // Directional light & Spot light
	    ImGui::DragFloat3("Direction", &_lights[selectedLight].direction[0], 0.1f);
	if (_lights[selectedLight].type == 2) // Spot light
		ImGui::DragFloat("Cutoff", &_lights[selectedLight].cutoff, 1.0f, 0.0f, 90.0f);


    ImGui::End();






    //ImGui::ShowDemoWindow();
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
	modelMatrix = glm::translate(glm::mat4(1.0f), _lights[selectedLight].position);

    ImGuizmo::Manipulate(
        glm::value_ptr(_camera->getViewMatrix()),
        glm::value_ptr(_camera->getProjectionMatrix()),
        mCurrentGizmoOperation,
        ImGuizmo::LOCAL,
        glm::value_ptr(modelMatrix)
    );


    // If modified, write back into your glm::mat4
    if (ImGuizmo::IsUsing()) {
		_lights[selectedLight].position = glm::vec3(modelMatrix[3]); 
    }
}

void LightManager::configShader(Shader& shader)
{
    for (int i{}; i < _lights.size(); i++) {
		Light& light = _lights[i]; 
		std::string prefix = "pointLights[" + std::to_string(i) + "].";
        shader.setVec3 (prefix + "position",     light.position);
        shader.setVec3 (prefix + "color",        light.color);
        shader.setFloat(prefix + "intensity",    light.intensity);
		shader.setInt  (prefix + "type",         light.type);
        shader.setVec3 (prefix + "direction",    light.color);
        shader.setFloat(prefix + "cutoff",       light.intensity);
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

	Light newLight;
	newLight.name = "Light " + std::to_string(lightCounter++);
    newLight.position = glm::vec3(0,0,0);
    newLight.color = glm::vec3(1,1,1);
    newLight.intensity = 1.0f;
    newLight.type = lightType; // 0=Point, 1=Directional, 2=Spot
    newLight.direction = glm::vec3(0,0,1);
    newLight.cutoff = 15; // spot için

	_lights.push_back(newLight);
}
//ImGui::SameLine();
