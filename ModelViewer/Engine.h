#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"
#include "Model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

class Engine
{
	GLFWwindow* _window;
	Renderer _renderer;
	Model _model;

	void initWindow();
	void initOpenGL();
	void init();
	void mainLoop();
	void terminate();

public:
	void run();
};

