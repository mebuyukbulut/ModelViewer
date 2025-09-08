#pragma once
#include <glm/glm.hpp>
class Particle; 

struct IForce {
    virtual glm::vec3 apply(Particle& p, float dt) = 0;
};
struct GravityForce : public IForce {
    glm::vec3 apply(Particle& p, float dt);
};
struct WindForce : public IForce {
    glm::vec3 direction{ 0.0f,0.0f,0.0f };
    glm::vec3 apply(Particle& p, float dt);
};
struct NoiseForce : public IForce {
    float strength{};
    glm::vec3 apply(Particle& p, float dt);
};
struct DragForce : public IForce {
    float strength{ 1 };
    glm::vec3 apply(Particle& p, float dt);
};
struct QuadraticDragForce : public IForce {
    float strength{ 5 };
    glm::vec3 apply(Particle& p, float dt);
};
