#include "LightManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Camera.h"

// I cannot find yet true formula for this conversation  
// for that reason I use that approximation
glm::vec3 kelvin2RGB_fast(float kelvin) {
    kelvin /= 100; 
    glm::vec3 color{ 0,0,0 };

    if (kelvin <= 66) {
        color.r = 255;

        color.g = kelvin;
        color.g = 99.4708025861 * std::log(color.g) - 161.1195681661;

        if (kelvin <= 19) {
            color.b = 0;
        }
        else {
            color.b = kelvin - 10;
            color.b = 138.5177312231 * std::log(color.b) - 305.0447927307;
        }
    }
    else {
        color.r = kelvin - 66;
        color.r = 329.698727446 * std::pow(color.r, -0.1332047592);

        color.g = kelvin - 60;
        color.g = 288.1221695283 * std::pow(color.g, -0.0755148492);

        color.b = 255;
    }

    return  glm::clamp(color / 255.f, glm::vec3(0), glm::vec3(1));
}

void Light::configShader(Shader& shader, std::string prefix) {
    shader.setVec3(prefix + "position", position);
    shader.setVec3(prefix + "color", color);
    shader.setFloat(prefix + "intensity", intensity);
    shader.setInt(prefix + "type", type);
}
void PointLight::configShader(Shader& shader, std::string prefix)
{
    Light::configShader(shader, prefix);
    shader.setFloat(prefix + "attenuation", attenuation);
}
void SpotLight::configShader(Shader& shader, std::string prefix)
{
    Light::configShader(shader, prefix);
    shader.setVec3(prefix + "direction", direction);
    shader.setFloat(prefix + "cutoff", cutoff);
    shader.setFloat(prefix + "attenuation", attenuation);
}
void DirectionalLight::configShader(Shader& shader, std::string prefix)
{
    Light::configShader(shader, prefix);
    shader.setVec3(prefix + "direction", direction);
}


void Light::drawUI()
{
    //ImGui::DragFloat3("Position", &position[0], 0.1f);
    ImGui::ColorEdit3("Color", &color[0]);
    static bool kelvinCB = false;
    static float kelvin{ 1000.0f };
    ImGui::Checkbox("Use temperature", &kelvinCB);
    if (kelvinCB) {
        ImGui::DragFloat("Kelvin", &kelvin, 10.0f, 1000.0f, 15'000.0f);
        color = kelvin2RGB_fast(kelvin);
    }

    ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.0f, 100.0f);
}
void PointLight::drawUI()
{
    Light::drawUI();
    ImGui::DragFloat("Attenuation", &attenuation, 0.1f, 0.1f, FLT_MAX);
}
void SpotLight::drawUI()
{
    Light::drawUI();
    ImGui::DragFloat3("Direction", &direction[0], 0.1f);
    ImGui::DragFloat("Cutoff", &cutoff, 1.0f, 0.0f, 90.0f);
    ImGui::DragFloat("Attenuation", &attenuation, 0.1f, 0.1f, FLT_MAX);
}
void DirectionalLight::drawUI()
{
    Light::drawUI();
    ImGui::DragFloat3("Direction", &direction[0], 0.1f);
}
