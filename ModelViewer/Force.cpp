#include "Force.h"
#include "ParticleSystem.h"
#include <glm/gtc/random.hpp>
//#include <glm/gtc/noise.hpp>

glm::vec3 GravityForce::apply(Particle& p, float dt){
       return glm::vec3(0.0f, -9.8f, 0.0f);
}
glm::vec3 WindForce::apply(Particle& p, float dt) {
    return direction;
}

glm::vec3 NoiseForce::apply(Particle& p, float dt) {
    glm::vec3 f{};
    f += glm::sphericalRand(strength);
    //f *= /*glm::perlin(glm::vec3{ p.age }) **/ strength;
    return f;
}

glm::vec3 DragForce::apply(Particle& p, float dt) {
    return p.velocity * -strength;
}

glm::vec3 QuadraticDragForce::apply(Particle& p, float dt) {
    return p.velocity * glm::length(p.velocity) * -strength;
}
