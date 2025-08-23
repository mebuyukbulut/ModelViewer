#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Shader.h"

struct Light {
    std::string name;
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    int type; // 0=Point, 1=Directional, 2=Spot
    glm::vec3 direction;
    float cutoff; // spot için
};
class LightManager
{
	const int MAX_LIGHTS = 8;
	std::vector<Light> _lights;
    int selectedLight = -1;
    class Camera* _camera = nullptr;
public:
	void init(Camera* camera) { _camera = camera; }
    void drawUI();
	void drawGizmo();
    void configShader(Shader& shader);

private:
    void addLight(int lightType);

 //   void removeLight(int index) {
 //       if (index >= 0 && index < _lights.size()) {
 //           _lights.erase(_lights.begin() + index);
 //       }
 //   }
 //   const std::vector<Light>& getLights() const {
 //       return _lights;
 //   }
 //   void clearLights() {
 //       _lights.clear();
	//}
    
    
};

