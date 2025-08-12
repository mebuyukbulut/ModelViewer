#pragma once
#include <string>
#include <functional>
struct GLFWwindow;

class UIManager
{
	GLFWwindow* _window;
public:
	void init(GLFWwindow* window);
	void terminate(); 

	void draw();

	bool isHoverOnUI();

private: 
	void beginFrame();
	void endFrame(); 

	bool isShaderPanelOpen = false;
	bool isCreditsPanelOpen = false;

	void mainMenu();
	void shaderPanel();
	void creditsPanel();

public:
	std::function<void(std::string)> onShaderSelected;
	std::function<void()> onEngineExit;
};

