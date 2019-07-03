#pragma once
#include <string>

#include <glm.hpp>

#include "Constants.h"

class Body {

	glm::vec3 pos;
	float mass;
	glm::vec3 vel;
	glm::vec3 acc;

public:
	Body(glm::vec3 p,
		glm::vec3 v,
		float mass);
	//~Body();
	double Mass();
	glm::vec3 Vel();
	glm::vec3 Acc();
	glm::vec3 Pos();
	void addG(Body& b);
	void update();
	void resetG();
	bool operator != (Body & b);
};