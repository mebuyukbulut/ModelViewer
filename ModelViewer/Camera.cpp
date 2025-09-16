#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Camera::init(glm::vec2 windowSize)
{
    _fov = glm::radians(45.0f); // 45 degrees in radians
    _windowSize = windowSize;
    _aspectRatio = windowSize.x / windowSize.y; // width / height
    _nearPlane = 0.1f; // near clipping plane
    _farPlane = 100.0f; // far clipping plane
    _position = glm::vec3(0.0f, 0.0f, 3.0f); // camera position
    _target = glm::vec3(0.0f, 0.0f, 0.0f); // camera target

}

glm::mat4 Camera::getProjectionMatrix()
{
    return glm::perspective(_fov, _aspectRatio, _nearPlane, _farPlane);
}

glm::mat4 Camera::getViewMatrix()
{   
    if (isDirty) updateVectors();
    return glm::lookAt(_position, _target, _up);
}

void Camera::setWindowSize(int width, int height){
	_windowSize.x = width; _windowSize.y = height;
	_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}
glm::vec2  Camera::getWindowSize(){ return _windowSize; }


void Camera::move(glm::vec3 offset)
{
}

void Camera::rotate(glm::vec3 angle) {
}

void Camera::zoom(float amount) {
}

void Camera::resetFrame()
{
    init(_windowSize);
}
