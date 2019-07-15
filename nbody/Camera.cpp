#pragma once
#include "Camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 upDir) : _pos(pos), _target(target), _upDir(upDir), _view(1.f)
{
	_front = glm::vec3(0.f, 0.f, -1.f);
	_direction = glm::normalize(_pos - _target);
	_right = glm::normalize(glm::cross(_upDir, _direction));
	_up = glm::cross(_direction, _right);

	_deltaTime = 0;
	_lastFrame = 0;
}

Camera::~Camera()
{

}

void Camera::update()
{
	_direction = glm::normalize(_pos - _target);
	_right = glm::normalize(glm::cross(_upDir, _direction));
	_up = glm::cross(_direction, _right);
	_view = glm::lookAt(_pos, glm::vec3(0.f, 0.f, 0.f), _up);

	const double currentFrame = glfwGetTime();
	_deltaTime = currentFrame - _lastFrame;
	_lastFrame = currentFrame;
}

glm::mat4 Camera::getView() noexcept
{
	return _view;
}

void Camera::input(GLFWwindow* window) noexcept
{
	const double speed = 1 * _deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		_pos = {0.f, 0.f, -5.f};
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		_pos = { 0.f, 0.f, 5.f };
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		_pos = { -5.f, 0.f, 0.f };
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		_pos = { 5.f, 0.f, 0.f };
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		_pos *= 1.05f;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		_pos *= 0.95f;
}