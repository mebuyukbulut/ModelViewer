#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Mesh.h"
#include "Material.h"

#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stb_image.h>
#include "Asset.h"
#include <future>
#include <memory>

class Texture;

// Load status for asynchronous model loading
enum class LoadStatus { None, Loading, ReadyToUpload, Complete, Error };
// loading status can divide to 2 status -> loading to CPU and loading to GPU

class Model : public Asset{

    MaterialManager* _materialManager{};
    //TextureManager* _textureManager{};
    
    std::vector<MaterialHandle> _materials{};
    std::vector<std::shared_ptr<Texture>> textures_loaded;
	
    std::vector<Mesh> meshes;
	//std::string _directory;
	//DefaultShapes _shape{ DefaultShapes::Cube };


    //unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = 1);
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:
    void draw(Shader& shader);
	void setMaterialManager(MaterialManager* materialManager) {
        _materialManager = materialManager;
        // if model loading not create a material use default one 
        if (_materials.empty()) {
            _materials.push_back(_materialManager->getDefaultMaterial());
        }
    }

    Model() { _type = AssetType::Model; }

    //LoadStatus loadFromFileAsync(const std::string& filename);
    //void updateLoadStatus();
    //LoadStatus getLoadStatus() const;



    // Inherited via Asset
    void load(std::filesystem::path path, IAssetSettings settings) override;

    void unload() override;

    void uploadToGPU() override;

};
