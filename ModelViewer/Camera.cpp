#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Camera::init()
{
    _fov = glm::radians(45.0f); // 45 degrees in radians
    _aspectRatio = 800.0f / 600.0f; // width / height
    _nearPlane = 0.1f; // near clipping plane
    _farPlane = 100.0f; // far clipping plane
    _position = glm::vec3(0.0f, 0.0f, 3.0f); // camera position
    _target = glm::vec3(0.0f, 0.0f, 0.0f); // camera target
    _up = glm::vec3(0.0f, 1.0f, 0.0f); // up vector
}

glm::mat4 Camera::getProjectionMatrix()
{
    return glm::perspective(_fov, _aspectRatio, _nearPlane, _farPlane);
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(_position, _target, _up);
}

void Camera::setAspectRatio(int width, int height)
{
	_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}
