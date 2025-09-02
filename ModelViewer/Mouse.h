#pragma once

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

	float _rotSens = .4f; // rotation sensitivity
	float _moveSens = 0.01f; // move sensitivity

public:
	void init(GLFWwindow* window, UIManager* UI);

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void mouse_cursor_callback(GLFWwindow* window, double xposIn, double yposIn);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

};

