#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <algorithm>
#include <glad/glad.h>
#include "Camera.h"

struct Particle {
    glm::vec3 position{ 0.0f, 0.0f, 0.0f };
    glm::vec3 velocity{ 0.0f, 0.0f, 0.0f };
    float lifetime{ 0.0f };
    float age{ 0.0f };
    float isactive{ 1.0f };

    void update(float deltaTime) {
        age += deltaTime;
        position = position + velocity * deltaTime;
        //gravity
        velocity.y -= deltaTime;// *0.1;
        //std::cout << position.x << std::endl; 
        if (age > lifetime) isactive = false;
    }
};
//struct EmitterInfo {
//    int spawnRate; // particle per second
//
//};

class ParticleSystem
{
    std::vector<Particle> _particles{};
    int _maxPCount;
    int _PCount{};
    Camera* _camera; 
    unsigned int _vao, _vbo;

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
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, lifetime));
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, age));
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, isactive));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);



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

        for (Particle& p : _particles) {
            if(p.isactive)
                p.update(deltaTime);
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


        float spawnRate = 100; // 100 particle per second 
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
                Particle newParticle;
                //newParticle.position;
                float rx = static_cast <float>(rand()) / static_cast <float>(RAND_MAX);
                float ry = static_cast <float>(rand()) / static_cast <float>(RAND_MAX);
                float rz = static_cast <float>(rand()) / static_cast <float>(RAND_MAX);
                rx += 0.5;
                //ry += 0.5;
                rz -= 0.5;

                rx*= 0.8;
                ry *= .2; ry += 0.9;
                rz*= 0.2;
                float rlife = static_cast <float>(rand()) / static_cast <float>(RAND_MAX);
                newParticle.velocity.x = 0.5;//rx;
                newParticle.velocity.y = ry;
                newParticle.velocity.z = rz;
                //newParticle.velocity.y = 0.1;
                newParticle.lifetime = 3 * rlife+3;
                _particles.push_back(newParticle);
            }
            //std::cout << "psize: " << _particles.size() << std::endl; 
        }

    }
};

