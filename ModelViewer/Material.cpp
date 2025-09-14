#include "Material.h"
#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// glm::vec4 baseColor;
// glm::vec4 emissive;
// float metallic;
// float roughness;
// float reflectance;
// float ao;


void Material::use(Shader* shader) {
	shader->setVec4("material.baseColor", baseColor);
	shader->setVec4("material.emissive", emissive);
	shader->setFloat("material.metallic", metallic);
	shader->setFloat("material.roughness", roughness);
	shader->setFloat("material.reflectance", reflectance);
	shader->setFloat("material.ao", ao);
}

void Material::drawUI() {
	ImGui::Begin("Material");
	
	ImGui::ColorEdit4("baseColor", glm::value_ptr(baseColor),ImGuiColorEditFlags_::ImGuiColorEditFlags_PickerHueWheel);
	
	ImGui::DragFloat("metallic", &metallic, 0.02f, 0.0f, 1.0f);
	ImGui::DragFloat("roughness", &roughness, 0.02f, 0.0f, 1.0f);
	ImGui::DragFloat("reflectance", &reflectance, 0.02f, 0.0f, 1.0f);
	ImGui::DragFloat("ao", &ao, 0.02f, 0.0f, 1.0f);
	ImGui::ColorEdit4("emissive", glm::value_ptr(emissive),ImGuiColorEditFlags_::ImGuiColorEditFlags_PickerHueWheel);
	
	ImGui::End();
}

