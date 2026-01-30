#include "Material.h"
#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <assimp/types.h>

#include <glad/glad.h>
#include "Texture.h"
#include "Logger.h"

// glm::vec4 baseColor;
// glm::vec4 emissive;
// float metallic;
// float roughness;
// float reflectance;
// float ao;


void Material::use(Shader* shader) {
	//glActiveTexture(GL_TEXTURE0);
	if (baseColorTexture) {
		//LOG_TRACE("base");
		glActiveTexture(GL_TEXTURE0);
		shader->setInt("baseColorTexture", 0); // Bu eksik!
		shader->setVec4("material.is_BaseColorTex", glm::vec4(1, 1, 1, 1)); // rgba -> r
		baseColorTexture->use();
	}
	else {
		shader->setVec4("material.is_BaseColorTex", glm::vec4(0, 0, 0, 0)); // reset
	}
	if (emissiveTexture) {
		glActiveTexture(GL_TEXTURE1);
		emissiveTexture->use();
		//shader->setInt("emissiveTexture", 0); // Bu eksik!
		shader->setVec4("material.is_EmissiveTex", glm::vec4(1, 0, 0, 0)); // rgba -> r
	}
	else {
		shader->setVec4("material.is_EmissiveTex", glm::vec4(0, 0, 0, 0)); // reset
	}
	if (metallicTexture) {
		glActiveTexture(GL_TEXTURE2);
		metallicTexture->use();
		//shader->setInt("metallicTexture", 0); // Bu eksik!
		shader->setVec4("material.is_MetallicTex", glm::vec4(1, 0, 0, 0)); // rgba -> r
	}
	else {
		shader->setVec4("material.is_MetallicTex", glm::vec4(0, 0, 0, 0)); // reset
	}
	if (roughnessTexture) {
		glActiveTexture(GL_TEXTURE3);
		roughnessTexture->use();
		//shader->setInt("roughnessTexture", 0); // Bu eksik!
		shader->setVec4("material.is_RoughnessTex", glm::vec4(1, 0, 0, 0)); // rgba -> r
	}
	else {
		shader->setVec4("material.is_RoughnessTex", glm::vec4(0, 0, 0, 0)); // reset
	}
	if (aoTexture) {
		glActiveTexture(GL_TEXTURE4);
		aoTexture->use();
		//shader->setInt("aoTexture", 0); // Bu eksik!
		shader->setVec4("material.is_AoTex", glm::vec4(1, 0, 0, 0)); // rgba -> r
	}
	else {
		shader->setVec4("material.is_AoTex", glm::vec4(0, 0, 0, 0)); // reset
	}

	shader->setVec4("material.baseColor", baseColor);
	shader->setVec4("material.emissive", emissive);
	shader->setFloat("material.metallic", metallic);
	shader->setFloat("material.roughness", roughness);
	shader->setFloat("material.reflectance", reflectance);
	shader->setFloat("material.ao", ao);
}

void Material::load(std::filesystem::path path, IAssetSettings settings)
{
	// dosyadan okuyup material oluştur. 
}

void Material::unload()
{
}

void Material::uploadToGPU()
{
}

void Material::onInspect()
{
	ImGui::SeparatorText("MATERIAL");
	ImGui::ColorEdit4("baseColor", glm::value_ptr(baseColor), ImGuiColorEditFlags_::ImGuiColorEditFlags_PickerHueWheel);

	ImGui::DragFloat("metallic", &metallic, 0.02f, 0.0f, 1.0f);
	ImGui::DragFloat("roughness", &roughness, 0.02f, 0.0f, 1.0f);
	ImGui::DragFloat("reflectance", &reflectance, 0.02f, 0.0f, 1.0f);
	ImGui::DragFloat("ao", &ao, 0.02f, 0.0f, 1.0f);
	ImGui::ColorEdit4("emissive", glm::value_ptr(emissive), ImGuiColorEditFlags_::ImGuiColorEditFlags_PickerHueWheel);
}


//
//// bu bindingler material sınıfında olmalı. 
//if (shader._type == Shader::Type::Foreground) {
//    // bind appropriate textures
//    unsigned int diffuseNr = 1;
//    unsigned int specularNr = 1;
//    unsigned int normalNr = 1;
//    unsigned int heightNr = 1;
//    for (unsigned int i = 0; i < _textures.size(); i++)
//    {
//        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
//        // retrieve texture number (the N in diffuse_textureN)
//        std::string number;
//        std::string name = _textures[i].get()->_type;
//        if (name == "texture_diffuse")
//            number = std::to_string(diffuseNr++);
//        else if (name == "texture_specular")
//            number = std::to_string(specularNr++); // transfer unsigned int to string
//        else if (name == "texture_normal")
//            number = std::to_string(normalNr++); // transfer unsigned int to string
//        else if (name == "texture_height")
//            number = std::to_string(heightNr++); // transfer unsigned int to string
//
//        // now set the sampler to the correct texture unit
//    //shader.setInt(name + number, i);
//        //std::cout << "texture name: " << (name+number) << std::endl;
//
//        // and finally bind the texture
//        glBindTexture(GL_TEXTURE_2D, _textures[i].id);
//    }
//}
