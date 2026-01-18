#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <yaml-cpp/yaml.h>

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "Shader.h"
#include "Component.h"
#include "Logger.h"

class Camera; 

enum class LightType { // 0=None, 1=Directional, 2=Point, 3=Spot
    None,
    Directional,
    Point,
    Spot
};

struct GPULight {
    glm::vec4 position;  // xyz + 1 float padding
    glm::vec4 color;     // rgb + intensity (intensity'yi dördüncü kanal yapabilirsin)
    glm::vec4 params;    // x: attenuation, y: type, z: cutoff, w: padding
    glm::vec4 direction; // xyz + 1 float padding

    GPULight() : position(0.0f), color(1.0f), params(0.0f), direction(0.0f) {} // incele kontrol et

	void setPosition(glm::vec3 pos)     { position = glm::vec4(pos, 1.0f); }
    void setColor(glm::vec3 col)        { color = glm::vec4(col, color.w); }
    void setIntensity(float intensity)  { color.w = intensity; }
    void setAttenuation(float attenuation) { params.x = attenuation; }
    void setType(LightType type)        { params.y = static_cast<float>(type); }// incele 
    void setCutoff(float cutoff)        { params.z = cutoff; }
    void setDirection(glm::vec3 dir)    { direction = glm::vec4(dir, 0.0f); }


    float* getPositionPtr() { return &position.x; }
    float* getColorPtr() { return &color.x; }
    float* getIntensityPtr() { return &color.w; }
    float* getAttenuationPtr() { return &params.x; }
    float* getTypePtr() { return &params.y; } // bunu değiştirmeli mi? 
    float* getCutoffPtr() { return &params.z; }
    float* getDirectionPtr() { return &direction.x; }


    glm::vec3 getPosition() { return glm::vec3(position); }
    glm::vec3 getColor() { return glm::vec3(color); }
    float getIntensity() { return color.w; }
    float getAttenuation() { return params.x; }
    LightType getType() { return static_cast<LightType>(params.y); }
    float getCutoff() { return params.z; }
    glm::vec3 getDirection() { return glm::vec3(direction); }
};

class Light : public Component {
protected:
    GPULight blockData;
    bool kelvinCB = false;
    float kelvin{ 1000.0f };

	// bool isDirty{ true };
public:

    Light();
	Light(GPULight data) : blockData(data) {}

    virtual void update();
    GPULight getGPULight() { 
		update(); // return isDirty ? update(), blockData : blockData;
        return blockData; 
    }

    //virtual void configShader(Shader& shader, std::string prefix);
    virtual YAML::Node serialize();
    virtual void onInspect() override;

    virtual ~Light() = default;
};
class PointLight : public Light {
public:

    PointLight();
    PointLight(GPULight data) : Light(data){}
    //void configShader(Shader& shader, std::string prefix) override;
    //YAML::Node serialize() override;
    void onInspect() override;
};
class SpotLight : public Light {
public:

    SpotLight();
    SpotLight(GPULight data) : Light(data) {}
    //void configShader(Shader& shader, std::string prefix) override;
    //YAML::Node serialize() override;
    void onInspect() override;

    void setDirection(glm::vec3 rotation);
};
class DirectionalLight : public Light {
public:

    DirectionalLight();
    DirectionalLight(GPULight data) : Light(data) {}
    //void configShader(Shader& shader, std::string prefix) override;
    //YAML::Node serialize() override;
    void onInspect() override;

};



class LightFactory {
public:
    static std::unique_ptr<Light> create(const YAML::Node& node);
};



class LightManager {
	const int MAX_LIGHTS = 8;
    struct GPULightBlock {
        GPULight lights[8];
        int numLights{};
        float padding[3]; // 16 byte'a tamamlamak için
    }blockData;

	GLuint uboLights; // Uniform Buffer Object ID


public:   
	LightManager();  // initialize UBO for lights 
    ~LightManager(); // cleanup UBO

	void queryLights(const std::vector<Light*> lights);

	std::unique_ptr<Light> createLight(LightType type);


};