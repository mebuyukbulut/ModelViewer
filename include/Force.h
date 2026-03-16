#pragma once
#include <glm/glm.hpp>
#include "Inspectable.h"

class Particle; 

struct IForce : public IInspectable{
    virtual glm::vec3 apply(Particle& p, float dt) = 0;
};
struct GravityForce : public IForce {
    float gravityScale = -9.8;
    glm::vec3 apply(Particle& p, float dt);

    // Inherited via IForce
    void drawUI() override;
};
struct WindForce : public IForce {
    glm::vec3 direction{ 0.0f,0.0f,0.0f };
    glm::vec3 apply(Particle& p, float dt);

    // Inherited via IForce
    void drawUI() override;
};
struct NoiseForce : public IForce {
    float strength{1};
    glm::vec3 apply(Particle& p, float dt);

    // Inherited via IForce
    void drawUI() override;
};
struct DragForce : public IForce {
    float strength{ 1 };
    glm::vec3 apply(Particle& p, float dt);

    // Inherited via IForce
    void drawUI() override;
};
struct QuadraticDragForce : public IForce {
    float strength{ 1 };
    glm::vec3 apply(Particle& p, float dt);

    // Inherited via IForce
    void drawUI() override;
};
