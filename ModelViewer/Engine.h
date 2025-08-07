#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"
#include "Model.h"
#include "Camera.h"

void processInput(GLFWwindow* window);

class Engine
{
	GLFWwindow* _window;
	Renderer _renderer;
	Model _model;
	Camera _camera;

	void initWindow();
	void initOpenGL();
	void init();
	void mainLoop();
	void terminate();
	void processInput(GLFWwindow* window);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

public:
	void run();
};

