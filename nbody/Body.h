#pragma once
#include <glm.hpp>

class Body {

private:
	glm::vec3 mPos;
	float mMass;
	glm::vec3 mVel;
	glm::vec3 mAcc;

public:
	Body() noexcept;
	Body(glm::vec3 pos,
		glm::vec3 vel,
		float mass);
	~Body();

	float getMass() noexcept;
	glm::vec3 getVel() noexcept;
	glm::vec3 getAcc() noexcept;
	glm::vec3 getPos() noexcept;
	void addForce(const glm::vec4& other);
	void update() noexcept;
	void resetForce() noexcept;
	void reset() noexcept;
	bool operator == (Body & b);
	bool operator != (Body& b);
	bool operator != (float& bMass);
};