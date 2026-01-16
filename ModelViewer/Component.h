#pragma once
#include "Object.h"

class Entity;
class Component : public Object {
public:
    Entity* owner{};
    virtual void onUpdate(float dt) {}
};

