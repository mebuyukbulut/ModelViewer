#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "Renderer.h"
#include "Model.h"
#include "Camera.h"
#include "UIManager.h"
void processInput(GLFWwindow* window);

class Engine
{
	GLFWwindow* _window;
	Renderer _renderer;
	//Model _model;
	std::vector<Model> _models;
	Camera _camera;
	UIManager _UI;

	float _mouseLastX, _mouseLastY;
	bool _mouseLeftPress;
	bool _firstMouse; 

	void initWindow();
	void initOpenGL();
	void init();

	void mainLoop();
	void terminate();
	void processInput(GLFWwindow* window);

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void mouse_cursor_callback(GLFWwindow* window, double xposIn, double yposIn);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

public:
	void run();
};

