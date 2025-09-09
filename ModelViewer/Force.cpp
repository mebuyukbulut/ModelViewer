#include "Force.h"
#include "ParticleSystem.h"
#include <glm/gtc/random.hpp>
//#include <glm/gtc/noise.hpp>
#include <imgui.h>

glm::vec3 GravityForce::apply(Particle& p, float dt){
       return glm::vec3(0.0f, gravityScale, 0.0f);
}
void GravityForce::drawUI()
{
    ImGui::DragFloat("Gravity scale", &gravityScale, 0.1);
}

glm::vec3 WindForce::apply(Particle& p, float dt) {
    return direction;
}
void WindForce::drawUI()
{
    ImGui::DragFloat3("Direction", &direction[0], 0.1);
}

glm::vec3 NoiseForce::apply(Particle& p, float dt) {
    glm::vec3 f{};
    f += glm::ballRand(strength);
    //f *= /*glm::perlin(glm::vec3{ p.age }) **/ strength;
    return f;
}
void NoiseForce::drawUI()
{
    ImGui::DragFloat("Noise strength", &strength, 0.1, 0.1, FLT_MAX, "%.3f", ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp);
}

glm::vec3 DragForce::apply(Particle& p, float dt) {
    return p.velocity * -strength;
}
void DragForce::drawUI()
{
    ImGui::DragFloat("Drag strength", &strength, 0.1, 0.1, FLT_MAX, "%.3f", ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp);
}

glm::vec3 QuadraticDragForce::apply(Particle& p, float dt) {
    return p.velocity * glm::length(p.velocity) * -strength;
}
void QuadraticDragForce::drawUI()
{
    ImGui::DragFloat("Quadratic Drag strength", &strength, 0.1, 0.1, FLT_MAX,"%.3f",ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp);
}
