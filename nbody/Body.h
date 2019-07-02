#pragma once
#include <string>

#include <glm.hpp>
#include <vec3.hpp>

#include "Constants.h"

class Body {

public:
	//Body();
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec3 vel;
	glm::vec3 acc;
	const double mass;
	Body(glm::vec3 p,
		glm::vec3 c,
		glm::vec3 v,
		const double mass);
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