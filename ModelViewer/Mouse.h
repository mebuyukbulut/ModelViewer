#pragma once
#include <functional>

struct GLFWwindow;
class UIManager;

class Mouse
{
	static Mouse* _this;
	static UIManager* _UI;

	float _mouseLastX = 0;
	float _mouseLastY = 0;
	bool _mouseLeftPress = false;
	bool _mouseRightPress = false;
	bool _firstMouse = true;

public:
	void init(GLFWwindow* window, UIManager* UI);

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void mouse_cursor_callback(GLFWwindow* window, double xposIn, double yposIn);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


	std::function<void(float dx, float dy)> onMove;
	std::function<void(float dx, float dy)> onRotate;
	std::function<void(float dy)> onZoom;
};

