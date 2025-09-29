#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "Shader.h"
#include "Inspectable.h"

class Camera; 
enum class LightType {
    Directional,
    Point,
    Spot
};
class Light : public IInspectable {
public:
    std::string name;
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    int type; // 0=Point, 1=Spot, 2=Directional

    Light() {
        std::cout << "Light constructor called \n";
        name = "light"; 
        position = glm::vec3(0, 0, 0); 
        color = glm::vec3(1, 1, 1);
        intensity = 1; 
        type = -1; 
    }
    virtual void configShader(Shader& shader, std::string prefix);

    // Inherited via IInspectable
    virtual void drawUI() override;
};
class PointLight : public Light {
public:
    float attenuation;
    
    PointLight() {
        std::cout << "Point Light constructor called \n";
        name = "Point Light";
        type = 0;
        attenuation = 3;
    }
    void configShader(Shader& shader, std::string prefix) override;
    void drawUI() override;
};
class SpotLight : public Light {
public:
    glm::vec3 direction;
    float cutoff;
    float attenuation;

    SpotLight() {
        std::cout << "Spot Light constructor called \n";
        name = "Spot Light";
        type = 1;

        direction = glm::vec3(0,-1,0);
        cutoff = 25;
        attenuation = 3;
    }
    void configShader(Shader& shader, std::string prefix) override;

    void drawUI() override;
};

class DirectionalLight : public Light {
public:
    glm::vec3 direction;

    DirectionalLight() {
        name = "Directional Light";
        type = 2;
        direction = glm::vec3(0, -1, 0);
    }
    void configShader(Shader& shader, std::string prefix) override;

    void drawUI() override;

};

