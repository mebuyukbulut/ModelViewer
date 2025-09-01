#include "Mouse.h"
#include "UIManager.h"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <ImGuizmo.h>

Mouse* Mouse::_this = nullptr; 
UIManager* Mouse::_UI = nullptr;
    
void Mouse::init(GLFWwindow* window, UIManager* UI)
{
    _this = this;
    _UI = UI; 

    glfwSetMouseButtonCallback(window, Mouse::mouse_button_callback);
    glfwSetCursorPosCallback(window, Mouse::mouse_cursor_callback);
    glfwSetScrollCallback(window, Mouse::scroll_callback);
}

void Mouse::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    //std::cout << _this << std::endl; 
    //if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS);
    //std::cout << _engine->_UI.isHoverOnUI() << std::endl;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !_UI->isHoverOnUI())
        _this->_mouseLeftPress = true;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        _this->_mouseLeftPress = false;
        _this->_firstMouse = true;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && !_UI->isHoverOnUI())
        _this->_mouseRightPress = true;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        _this->_mouseRightPress = false;
        _this->_firstMouse = true;
    }

}
void Mouse::mouse_cursor_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    //Engine* _engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    //if (!_engine) return;
    if (!(_this->_mouseLeftPress || _this->_mouseRightPress)) return;

    if (ImGuizmo::IsUsing()) {
        _this->_mouseLeftPress = false;
        _this->_mouseRightPress = false;
        _this->_firstMouse = true;
        return;
    }


    if (_this->_firstMouse) {
        _this->_mouseLastX = xposIn;
        _this->_mouseLastY = yposIn;
        _this->_firstMouse = false;
        return;
    }

    float xoffset = xposIn - _this->_mouseLastX;
    float yoffset = _this->_mouseLastY - yposIn; // reversed since y-coordinates go from bottom to top

    _this->_mouseLastX = xposIn;
    _this->_mouseLastY = yposIn;

    if (_this->_mouseLeftPress) {
        xoffset = glm::radians(.4f) * xoffset;
        yoffset = glm::radians(.4f) * yoffset;
        //std::cout << xoffset << "\t" << yoffset << std::endl;
        _this->onRotate(xoffset, yoffset);
    }
    else { //_mouseRightPress
        _this->onMove(xoffset * 0.01f, yoffset * 0.01f);
    }
}
void Mouse::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    if (!_UI->isHoverOnUI())
        _this->onZoom(yoffset);
}
