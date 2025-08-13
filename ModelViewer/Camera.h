#pragma once
#include <glm/glm.hpp>

class Camera
{
	float _fov; // field of view
	float _aspectRatio; // width / height
	float _nearPlane; // near clipping plane
	float _farPlane; // far clipping plane
	glm::vec3 _position; // camera position
	glm::vec3 _target; // camera target
	glm::vec3 _up; // up vector
	glm::vec3 _centerPoint;

	float _radius, _theta, _phi; // spherical coordinates for camera position

public:
	void init();

	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();
	
	glm::vec3 getPosition() const { return _position; }

	void setAspectRatio(int width, int height);

	void move(float dX, float dY);
	void rotate(float dTheta, float dPhi);
	void zoom(float dr);
};

