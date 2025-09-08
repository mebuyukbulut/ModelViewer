#include "EmitterShape.h"

#include <glm/gtc/random.hpp>
//#include <glm/gtc/noise.hpp>


glm::vec3 PointShape::samplePosition() {
    return glm::vec3();
}
glm::vec3 SphereShape::samplePosition() {
    return glm::sphericalRand(radius);
}
glm::vec3 TorusShape::samplePosition() {
    glm::vec2 bigOne = glm::circularRand(radius);
    glm::vec3 littleOne = glm::sphericalRand(innerRadius);//glm::diskRand(innerRadius);
    glm::vec3 a{ bigOne.x, 0, bigOne.y };
    return a + littleOne;
    //return glm::sphericalRand(radius);
}