#pragma once
#include "Camera.h"


Camera::Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 upDir) : _pos(pos), _target(target), _upDir(upDir)
{
	_front = glm::vec3(0.f, 0.f, -1.f);
}

Camera::~Camera()
{

}

void Camera::update()
{
	_direction = glm::normalize(_pos - _target);
	_right = glm::normalize(glm::cross(_up, _direction));
	_up = glm::cross(_direction, _right);
}

glm::mat4 Camera::getView()
{
	return glm::lookAt(_pos, _pos + _front, _up);
}

void Camera::moveF()
{
	_pos += 0.01f * _front;
}
void Camera::moveB()
{
	_pos -= 0.01f * _front;
}
void Camera::moveL()
{
	_pos -= glm::normalize(glm::cross(_front, _up))*0.01f;
}
void Camera::moveR()
{
	_pos += glm::normalize(glm::cross(_front, _up))*0.01f;
}