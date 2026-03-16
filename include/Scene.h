#pragma once
#include <vector>
#include "Material.h"
#include "LightManager.h"

class Scene {
public:
    std::string name;

    // Sahnenin sahip olduğu kaynaklar
    //std::vector<MaterialHandle> materials;
    std::vector<Light> lights;

    // TODO: entity list, camera list, vs.

    bool load(const std::string& path);   // YAML/JSON’dan yükle
    bool save(const std::string& path);   // dosyaya yaz

    void unload(); // tüm objeleri/materialleri temizle
};