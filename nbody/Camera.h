#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Constants.h"

class Camera
{
public:

	Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up);
	~Camera();
	void update();
	glm::mat4 getView();
	void moveF();
	void moveB();
	void moveL();
	void moveR();

private:

	glm::vec3 _pos;
	glm::vec3 _target;
	glm::vec3 _direction;
	glm::vec3 _upDir;
	glm::vec3 _right;
	glm::vec3 _up;
	glm::vec3 _front;

	glm::mat4 projection;
	glm::mat4 view;

};