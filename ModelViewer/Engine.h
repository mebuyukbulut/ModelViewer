#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "Renderer.h"
#include "Model.h"
#include "Camera.h"
#include "UIManager.h"
#include "LightManager.h"
#include "Time.h"
#include "Mouse.h"
#include "Config.h"
#include "ParticleSystem.h"

class Engine
{
	GLFWwindow* _window;
	Renderer _renderer;
	std::vector<Model> _models;
	Camera _camera;
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

