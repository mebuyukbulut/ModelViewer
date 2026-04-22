#include "Material.h"
#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <assimp/types.h>

#include <glad/gl.h>
#include "Texture.h"
#include "Logger.h"
#include "Builtin.h"

#include "AssetManager.h"
// glm::vec4 baseColor;
// glm::vec4 emissive;
// float metallic;
// float roughness;
// float reflectance;
// float ao;


// her seferinde hem değerleri, hem textureları, hem de flag ları bind edelim


void Material::use(Shader* shader) {
	// Set Texture location uniforms 
	// We should do this for once. Because we always bind the same slot.  
	shader->set(Builtin::Material::BaseColorTexture, Builtin::TextureSlot::BaseColor); 
	shader->set(Builtin::Material::ARMTexture, 		 Builtin::TextureSlot::ARM); 
	shader->set(Builtin::Material::NormalTexture,	 Builtin::TextureSlot::Normal); 
	shader->set(Builtin::Material::EmissiveTexture,  Builtin::TextureSlot::Emissive); 

	if(!baseColorTexture){
		baseColorTexture = g_Assets.get<Texture>("../assets/textures/box_crate.jpg");
	}

	// Bind Textures
	(baseColorTexture ? baseColorTexture : defTex.white )->bind(Builtin::TextureSlot::BaseColor);
	(armTexture 	  ? armTexture : 	   defTex.white )->bind(Builtin::TextureSlot::ARM);
	(normalTexture	  ? normalTexture :    defTex.normal)->bind(Builtin::TextureSlot::Normal);
	(emissiveTexture  ? emissiveTexture :  defTex.white )->bind(Builtin::TextureSlot::Emissive);



	shader->set(Builtin::Material::BaseColor, 	 	baseColor);
	shader->set(Builtin::Material::Emissive, 	 	emissive);
	shader->set(Builtin::Material::Metallic, 	 	metallic);
	shader->set(Builtin::Material::Roughness, 	 	roughness);
	shader->set(Builtin::Material::Reflectance,  	reflectance);
	shader->set(Builtin::Material::AO, 			 	ao);
}

void Material::load(std::filesystem::path path, IAssetSettings* settings)
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


Material::DefaultTextures::DefaultTextures()
{
	white  = g_Assets.get<Texture>(Builtin::Texture::SolidWhite);
	black  = g_Assets.get<Texture>(Builtin::Texture::SolidBlack);
	normal = g_Assets.get<Texture>(Builtin::Texture::FlatNormal);
}
