#pragma once
#include "Object.h"

class Entity;
class Component : public Object {
public:
    Entity* owner{};
    virtual void onUpdate(float dt) {}

    // Inherited via Object
    virtual void serialize(YAML::Emitter& out) override;
    virtual void deserialize(const YAML::Node& node) override;
};

