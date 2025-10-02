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


class Model
{
    MaterialManager* _materialManager{};
    //TextureManager* _textureManager{};
    std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<Mesh> meshes;
	std::string _directory;
    std::vector<MaterialHandle> _materials{};

    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = 1);
    bool loadModel(std::string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:
    void draw(Shader& shader);
    void terminate();
    void loadDefault();
    bool loadFromFile(const std::string& filename);

    Model(MaterialManager* materialManager) : _materialManager{ materialManager } {}
};

