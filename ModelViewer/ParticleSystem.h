#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>

#include <vector>
#include <iostream>
#include <algorithm>
#include <memory>

#include "Inspectable.h"
#include "Camera.h"
#include "ColorProvider.h"
#include "Force.h"
#include "EmitterShape.h"
 
struct Particle {
    glm::vec3 position{ 0.0f, 0.0f, 0.0f };
    glm::vec3 velocity{ 0.0f, 0.0f, 0.0f };
    glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
    float lifetime{ 0.0f };
    float age{ 0.0f };
    float isactive{ 1.0f };

    std::shared_ptr<IColorProvider> _colorFunction;


    void init(std::shared_ptr<IColorProvider> colorFunction){
        _colorFunction = colorFunction;
    }
    void update(float deltaTime, float elapsedTime); 
};


struct EmitterInfo : public IInspectable{
    int spawnRate{}; // particle per second
    std::unique_ptr<IEmitterShape> shape;

    EmitterInfo() : spawnRate{ 100 }, shape{new PointShape} {}
    EmitterInfo(int spawnRate, std::unique_ptr<IEmitterShape> shape) 
        : spawnRate{ spawnRate }, shape{ std::move(shape) } {}
    void drawUI();
};

class ParticleSystem : public IInspectable
{
    std::vector<Particle> _particles{};
    int _maxPCount;
    int _PCount{};
    Camera* _camera; 
    unsigned int _vao, _vbo;

    EmitterInfo info{ 100, std::make_unique<TorusShape>()};

    Particle emit() {
        Particle newParticle;


        newParticle.position = info.shape->samplePosition();
        GradientProvider gp; 
        gp.addStop(GradientProviderStop{ 0,     glm::vec4(0,1,0,1) });
        gp.addStop(GradientProviderStop{ 0.25,  glm::vec4(1,0,1,1) });
        gp.addStop(GradientProviderStop{ 0.75,  glm::vec4(0,1,1,1) });
        gp.addStop(GradientProviderStop{ 1,     glm::vec4(1,1,0,1) });

        std::shared_ptr<IColorProvider> cp;
        //cp.reset(new GradientProvider());
        cp = std::make_shared<GradientProvider>(gp);
        newParticle.init(cp);


        float rlife = static_cast <float>(rand()) / static_cast <float>(RAND_MAX);
        //newParticle.velocity.y = 0.1;
        //newParticle.lifetime = 3 * rlife + 3;
        newParticle.lifetime = 20;
        return newParticle;
    }

public:
    void init(Camera* camera){ 
        _maxPCount = 10'000;
        _camera = camera; 
        glEnable(GL_PROGRAM_POINT_SIZE);

        //_maxPCount = maxParticleCount;
        //_PCount = 0;


        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(_vao);

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * _maxPCount, nullptr, GL_STATIC_DRAW);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * _particles.size(), _particles.data(), GL_STATIC_DRAW);


        //glm::vec3 position{ 0.0f, 0.0f, 0.0f };
        //glm::vec3 velocity{ 0.0f, 0.0f, 0.0f };
        //float lifetime{ 0.0f };
        //float age{ 0.0f };
        //bool isactive{ true };


        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, position));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, velocity));
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, color));
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, lifetime));
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, age));
        glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, isactive));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);



        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
    }

    void draw() {
        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Particle) * _particles.size(), _particles.data());
        //glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * _particles.size(), _particles.data(), GL_STATIC_DRAW);
        //std::cout << "draw _p.size(): " << _particles.size() << std::endl;
        //std::cout << _particles[0].position.x << std::endl;
        glDrawArrays(GL_POINTS, 0, _particles.size());
    }

    void update(double deltaTime) {
        static float elapsedTime = 0;
        elapsedTime += deltaTime;
        for (Particle& p : _particles) {
            if(p.isactive)
                p.update(deltaTime, elapsedTime);
        }
        //for (int i{}; i < _particles.size();) {
        //    if (_particles[i].isactive)
        //        i++;
        //    else
        //        _particles.erase(_particles.begin() + i);
        //}

        _PCount = _particles.size();
        auto it = std::remove_if(_particles.begin(), _particles.end(), 
            [&](Particle p) {
                return !p.isactive;
            }
        );
        _particles.erase(it, _particles.end());


        float spawnRate = info.spawnRate; // 100 particle per second 
        static float newParticleCount = 0;
        newParticleCount += spawnRate * deltaTime;
        //std::cout << "Dtime: " << deltaTime << std::endl;
        //std::cout << newParticleCount << std::endl;
        //std::cout << "update _p.size(): " << _particles.size() << std::endl;

        if (newParticleCount > 1) {
            int N = newParticleCount;
            newParticleCount -= N;

            for (int i{}; i < N; i++) {
                if (_particles.size() + 1 >= _maxPCount)
                    break;

                _particles.push_back(emit());
            }
            //std::cout << "psize: " << _particles.size() << std::endl; 
        }

    }

    void drawUI();
};

