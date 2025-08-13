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
	_centerPoint = glm::vec3(0.0f, 0.0f, 0.0f); // center point of the camera orbit

	// Spherical coordinates for camera position
    _radius = 5.0f; // distance from the target
    _theta = glm::radians(45.0f); // angle in the x-z plane
    _phi = glm::radians(45.0f); // angle from the y-axis

}

glm::mat4 Camera::getProjectionMatrix()
{
    return glm::perspective(_fov, _aspectRatio, _nearPlane, _farPlane);
}

glm::mat4 Camera::getViewMatrix()
{    
    // Update camera position based on spherical coordinates
    _position.x = _radius * sin(_phi) * cos(_theta);
    _position.y = _radius * cos(_phi);
	_position.z = _radius * sin(_phi) * sin(_theta);
    return glm::lookAt(_position + _centerPoint, _target + _centerPoint, _up);
}

void Camera::setAspectRatio(int width, int height)
{
	_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

void Camera::move(float dX, float dY)
{
	glm::vec3 front = glm::normalize(_target - _position); // direction from camera to target
	glm::vec3 right = glm::normalize(glm::cross(front, _up)); // right vector
	glm::vec3 up = glm::normalize(glm::cross(right, front)); // up vector

 //   glm::vec3 myvec(dX, dY, 0);
 //   auto myMat = glm::lookAt(_position, _target, up);
	//auto a = glm::transpose(glm::inverse(glm::mat3(myMat))) * myvec; // transform the vector to world space
 //   
    _centerPoint += -right * dX - up * dY;
}

void Camera::rotate(float dTheta, float dPhi) {
    _theta += dTheta;
    _phi = glm::clamp(_phi + dPhi, 0.1f, glm::pi<float>() - 0.1f); // avoid flipping
}

void Camera::zoom(float dr) {
    _radius = glm::max(0.1f, _radius + dr);
}