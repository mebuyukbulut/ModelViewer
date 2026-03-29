#pragma once
#include <glm/glm.hpp>
#include "IInspectable.h"

struct IEmitterShape : public IInspectable {
    virtual glm::vec3 samplePosition() = 0;

    virtual void onInspect() override {}
};
struct PointShape : IEmitterShape {
    glm::vec3 samplePosition();

    // Inherited via IEmitterShape
    void onInspect() override;
};
struct SphereShape : IEmitterShape {
    float radius{ 0.1f };
    glm::vec3 samplePosition();

    // Inherited via IEmitterShape
    void onInspect() override;
};
struct TorusShape : IEmitterShape {
    float innerRadius{ 0.1f };
    float radius{ 1.0f };

    glm::vec3 samplePosition();

    // Inherited via IEmitterShape
    void onInspect() override;
};


