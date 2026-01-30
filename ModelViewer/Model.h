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

class Model : public Asset{

    //MaterialManager* _materialManager{};
    //TextureManager* _textureManager{};
    
    std::vector<std::shared_ptr<Material>> _materials{};
	
    std::vector<Mesh> meshes;
	//std::string _directory;
	//DefaultShapes _shape{ DefaultShapes::Cube };

    void loadDefault(DefaultShapes shape);

    //unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = 1);
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::shared_ptr<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type);

public:
    Model() { _type = AssetType::Model; }

    void draw(Shader& shader);

    // Inherited via Asset
    void load(std::filesystem::path path, IAssetSettings settings) override;
    void unload() override;
    void uploadToGPU() override;

    virtual void onInspect() {
        if (_materials.size())
            _materials.front().get()->onInspect();
    }

};
