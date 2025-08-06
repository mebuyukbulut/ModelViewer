#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

class Engine
{
	GLFWwindow* window;

	void initWindow();
	void initOpenGL();
	void init();

public:
	void run() { init(); }
};

