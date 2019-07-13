#pragma once
#include <string>
#include <random>

#include <glm.hpp>

#include "Constants.h"

class Body {

private:
	glm::vec3 mPos;
	float mMass;
	glm::vec3 mVel;
	glm::vec3 mAcc;

public:
	Body();
	Body(glm::vec3 pos,
		glm::vec3 vel,
		float mass);
	~Body();

	double getMass();
	glm::vec3 getVel();
	glm::vec3 getAcc();
	glm::vec3 getPos();
	void addForce(const glm::vec4& other);
	void update();
	void resetForce();
	void reset();
	bool operator != (Body & b);
};