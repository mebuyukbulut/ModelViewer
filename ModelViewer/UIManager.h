#pragma once
#include <string>
#include <functional>
#include <glm/glm.hpp>
#include "Camera.h"

struct GLFWwindow;
class LightManager;
class Camera;
class Material;

class UIManager
{
	GLFWwindow* _window;
	Camera* _camera;
	LightManager* _lightManager;
	//glm::vec2 _windowSize;
public:
	void init(GLFWwindow* window, LightManager* lightManager, Camera* camera);
	//void setWindowSize(int width, int height) { _windowSize = glm::vec2(width, height); }

	void terminate(); 

	void draw(Material* material);

	bool isHoverOnUI();

	class ParticleSystem* ps;

private: 
	void beginFrame();
	void endFrame(); 

	bool isShaderPanelOpen = false;
	bool isCreditsPanelOpen = false;
	bool isLightPanelOpen = false; 
	bool isMaterialPanelOpen = false; 

	void mainMenu();
	void shaderPanel();
	void creditsPanel();


};

