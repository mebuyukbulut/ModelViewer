#include "ParticleSystem.h"

glm::vec3 lerp(const glm::vec3& x, const glm::vec3& y, float t) {
    return x * (1.f - t) + y * t;
}
glm::vec3 bezier(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, float t) {
    glm::vec3 ab = lerp(a, b, t);
    glm::vec3 bc = lerp(b, c, t);
    glm::vec3 cd = lerp(c, d, t);

    glm::vec3 ab_bc = lerp(ab, bc, t);
    glm::vec3 bc_cd = lerp(bc, cd, t);

    return lerp(ab_bc, bc_cd, t);
}

void Particle::update(float deltaTime, float elapsedTime) {
    age += deltaTime;
    float normalizedAge = age / lifetime;
    //float mass = 1;
    //glm::vec3 totalForce{};
    //glm::vec3 acceleration{};
    //std::vector<std::unique_ptr<IForce>> forces;
    //WindForce wf{};
    //GravityForce gf{};
    //NoiseForce nf{};

    //wf.direction = glm::vec3(1.0f, 0.0f, 0.0f);
    //nf.strength = 3;

    ////forces.push_back(std::make_unique<WindForce>(wf));
    //forces.push_back(std::make_unique<NoiseForce>(nf));
    //forces.push_back(std::make_unique<GravityForce>(gf));
    //forces.push_back(std::make_unique<QuadraticDragForce>());
    //
    //for (auto& f : forces)
    //    totalForce += f->apply(*this, deltaTime);

    //acceleration = totalForce / mass; 
    //velocity += acceleration * deltaTime;
    //position += velocity * deltaTime;

    glm::vec3 a{ 0.0f, 0.0f, 0.0f };
    glm::vec3 b{ 5.0f, 0.0f, 0.0f };
    glm::vec3 c{ -5.0f, 0.0f, 10.0f };
    glm::vec3 d{ 0.0f, 0.0f, 10.0f };
    float t = age / lifetime;
    position = bezier(a, b, c, d, t);












    //// https://libnoise.sourceforge.net/tutorials/tutorial2.html
    //noise::module::Perlin myModule;
    ////noise::module::random
    //
    //float a = glm::linearRand(0.0f, 10.0f);
    //
    //position.y = glm::perlin(glm::vec3((a + elapsedTime) * 2, 0, 0));
    //position.x = a;

    color = _colorFunction->evaluate(normalizedAge);

    //color.r = position.y + 0.5;
    //color.g = 0;
    //color.b = 1 - position.y - 0.2;


    //// perlin
    ////glm::vec1 nv {position.y}; 
    //glm::vec3 p = position; //+ glm::vec3(age * 0.5f);
    //glm::perlin(p);
    //// Use perlin with offsets for each axis
    //glm::vec3 force(
    //    (float)glm::perlin(p), //+ glm::vec3(31.4f, 0.0f, 0.0f)),
    //    0,//(float)glm::perlin(p), //+ glm::vec3(0.0f, 71.2f, 0.0f)),
    //    (float)glm::perlin(p) //+ glm::vec3(0.0f, 0.0f, 19.7f))
    //);
    //position += force;
    ////velocity += force * 0.05f;

    ////gravity
    //velocity.y -= deltaTime;// *0.1;


    //std::cout << position.x << std::endl; 
    if (age > lifetime) isactive = false;
}