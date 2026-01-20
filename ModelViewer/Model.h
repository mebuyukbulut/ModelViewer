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
#include "Component.h"
#include <future>

class Texture;

// Load status for asynchronous model loading
enum class LoadStatus { None, Loading, ReadyToUpload, Complete, Error };
// loading status can divide to 2 status -> loading to CPU and loading to GPU

class Model : public Component
{
    MaterialManager* _materialManager{};
    //TextureManager* _textureManager{};

    std::vector<MaterialHandle> _materials{};
    std::vector<Texture*> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<Mesh> meshes;
	std::string _directory;
    std::string _path{};

	std::future<LoadStatus> _asyncLoadStatus = std::future<LoadStatus>();
	LoadStatus _loadStatus = LoadStatus::None;

    //unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = 1);
    LoadStatus loadModel(std::string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:
    void draw(Shader& shader);
    void terminate();
    void loadDefault(DefaultShapes shape = DefaultShapes::Cube);
    LoadStatus loadFromFile(const std::string& filename);
    void addMesh(Mesh mesh) {
        meshes.push_back(mesh);
        if (_materials.empty()) {
            _materials.push_back(_materialManager->getDefaultMaterial());
        }
    }
    std::string getPath() { return _path; }


    Model(MaterialManager* materialManager) : _materialManager{ materialManager } {}

    LoadStatus loadFromFileAsync(const std::string& filename);
    void updateLoadStatus();
    LoadStatus getLoadStatus() const;

	void onInspect() override;
    void serialize(YAML::Emitter& out) override;
    void deserialize(const YAML::Node& node) override;
};

