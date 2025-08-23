#pragma once
#include <string>
#include <functional>
struct GLFWwindow;
class LightManager;

class UIManager
{
	GLFWwindow* _window;
	LightManager* _lightManager;
public:
	void init(GLFWwindow* window, LightManager* lightManager);
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
	std::function<void(std::string)> onOpenModel;
	std::function<void()> onEngineExit;
};

