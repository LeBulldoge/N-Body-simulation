#pragma once
#include <string>
#include <random>

#include <glm.hpp>

#include "Constants.h"

class Body {

	glm::vec3 pos;
	float mass;
	glm::vec3 vel;
	glm::vec3 acc;

public:
	Body();
	Body(glm::vec3 p,
		glm::vec3 v,
		float mass);
	~Body();

	static std::random_device seed;
	static std::mt19937 rng;
	static std::uniform_real_distribution<> dis;
	static std::uniform_real_distribution<> disMass;

	double Mass();
	glm::vec3 Vel();
	glm::vec3 Acc();
	glm::vec3 Pos();
	void addG(const glm::vec4& other);
	void update();
	void resetG();
	bool operator != (Body & b);
};