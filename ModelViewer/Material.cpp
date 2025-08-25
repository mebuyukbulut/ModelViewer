#include "Material.h"
#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void Material::use(Shader* shader) {
	shader->setVec3("material.color", _color);
	shader->setFloat("material.specularIntensity", _specular);	
}

void Material::drawUI() {
	ImGui::Begin("Material");
	
	ImGui::ColorEdit3("Color", glm::value_ptr(_color),ImGuiColorEditFlags_::ImGuiColorEditFlags_PickerHueWheel);
	ImGui::DragFloat("Intensity", &_specular, 0.02f, 0.0f, 1.0f);

	ImGui::End();
}

