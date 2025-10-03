#include "Scene.h"
#include <yaml-cpp/yaml.h>

bool Scene::load(const std::string& path)
{
    YAML::Node root = YAML::LoadFile(path);

    if (!root["Scene"]) return false;

    name = root["Scene"]["name"].as<std::string>();

    // pos, rot, scale, 
    // color 
    // use temperature? 
    // temperature Kelvin
    // intensity
    // Direction 
    // cutoff
    // attenuation 
    // type

    

    // Load Lights
    if (root["Lights"]) {
        for (const auto& node : root["Lights"]) {
            Light light;
            auto pos = node["position"].as<std::vector<float>>();
            auto col = node["color"].as<std::vector<float>>();
            light.position = { pos[0], pos[1], pos[2] };
            light.color = { col[0], col[1], col[2] };
            light.intensity = node["intensity"].as<float>();
            light.type = node["type"].as<int>();
            lights.push_back(light);
        }
    }
    // base color 
    // metallic
    // roughness
    // reflectance
    // ao
    // emissive
    

    // Load Materials
    if (root["Materials"]) {
        for (const auto& node : root["Materials"]) {
            Material mat;
            auto base = node["baseColor"].as<std::vector<float>>();
            mat.baseColor = { base[0], base[1], base[2], base[3] };
            mat.metallic = node["metallic"].as<float>();
            mat.roughness = node["roughness"].as<float>();
            //materials.push_back(mat);
        }
    }
}
bool Scene::save(const std::string& path)
{
    return false;
}
