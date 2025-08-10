#pragma once
#include <string>
#include <functional>
struct GLFWwindow;

class UIManager
{
public:
	void init(GLFWwindow* window);
	void terminate(); 

	void draw();

private: 
	void beginFrame();
	void endFrame(); 

public:
	std::function<void(std::string)> onShaderSelected;
};

