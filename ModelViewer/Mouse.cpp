#include "Mouse.h"
#include "UIManager.h"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <ImGuizmo.h>
#include "EventDispatcher.h"


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
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !_UI->isHoverOnUI()) {
        _this->_mouseLeftPress = true;

        Event e{ EventType::Select };
        ImVec2 mousePos = ImGui::GetMousePos();
        e.data.vec = glm::vec3(mousePos.x, mousePos.y, 0);
        dispatcher.dispatch(e);
    }
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
        xoffset = glm::radians(_this->_rotSens) * xoffset;
        yoffset = glm::radians(_this->_rotSens) * yoffset;
        glm::vec3 vec(xoffset, yoffset, 0);

        Event e{ EventType::onRotate, EventData{glm::vec3(xoffset,yoffset,0)}};
        dispatcher.dispatch(e);
    }
    else { //_mouseRightPress
        glm::vec3 vec(xoffset * _this->_moveSens, yoffset * _this->_moveSens, 0);
        Event e{ EventType::onMove, EventData{vec} };
        dispatcher.dispatch(e);
    }
}
void Mouse::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    if (!_UI->isHoverOnUI()) {
        glm::vec3 vec(0, yoffset, 0);
        Event e{ EventType::onZoom, EventData{vec} };
        dispatcher.dispatch(e);
    }
}
