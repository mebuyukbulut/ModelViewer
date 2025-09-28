#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <memory>

#include "Renderer.h"
#include "Model.h"
#include "Camera.h"
#include "UIManager.h"
#include "LightManager.h"
#include "Time.h"
#include "Mouse.h"
#include "Config.h"
#include "ParticleSystem.h"
#include "Entity.h"
class Engine
{
	
	GLFWwindow* _window;
	Renderer _renderer;
	std::list<std::shared_ptr<Transform>> _transforms;
	Transform* selected_transform{};

	std::shared_ptr<Camera> _camera{new OrbitCamera()};
	UIManager _UI;
	LightManager _lightManager;
	Mouse _mouse;
	Time time;
	//Config config;
	ParticleSystem ps; 

	void initWindow();
	void initOpenGL();
	void initUI();
	void init();

	void mainLoop();
	void terminate();
	void processInput(GLFWwindow* window);

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);


public:
	void run();
};

