#include "EmitterShape.h"

#include <glm/gtc/random.hpp>
//#include <glm/gtc/noise.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

glm::vec3 PointShape::samplePosition() {
    return glm::vec3();
}
void PointShape::drawUI()
{
    ImGui::SeparatorText("Point");
}
glm::vec3 SphereShape::samplePosition() {
    return glm::sphericalRand(radius);
}
void SphereShape::drawUI()
{
    ImGui::SeparatorText("Sphere");
    ImGui::DragFloat("Radius", &radius, 0.1, 0.1, FLT_MAX);

}
glm::vec3 TorusShape::samplePosition() {
    glm::vec2 bigOne = glm::circularRand(radius);
    glm::vec3 littleOne = glm::sphericalRand(innerRadius);//glm::diskRand(innerRadius);
    glm::vec3 a{ bigOne.x, 0, bigOne.y };
    return a + littleOne;
    //return glm::sphericalRand(radius);
}

void TorusShape::drawUI()
{
    ImGui::SeparatorText("Torus");
    ImGui::DragFloat("Radius", &radius, 0.1, 0.1, FLT_MAX);
    ImGui::DragFloat("Inner radius", &innerRadius, 0.1, 0.1, FLT_MAX);

}
