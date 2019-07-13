#include "Body.h"



Body::Body() : Body({0.f, 0.f, 0.f}, { 0.f, 0.f, 0.f }, 0.f)
{

}


Body::Body(glm::vec3 pos, glm::vec3 vel , float mass) : mPos(pos), mVel(vel), mMass(mass)
{
	mAcc = { 0.f, 0.f, 0.f };
}

Body::~Body()
{

}

double Body::getMass()
{
	return mMass;
}
glm::vec3 Body::getAcc()
{
	return mAcc;
}
glm::vec3 Body::getVel()
{
	return mVel;
}
glm::vec3 Body::getPos()
{
	return mPos;
}

void Body::update()
{
	//Velocity
	mVel += mAcc * step;
	//Position
	mPos += mVel * step;
}

void Body::addForce(const glm::vec4& other)
{
	//Distance vector
	glm::vec3 dist = glm::vec3(other) - mPos;
	//Normalized distance
	float r2 = glm::dot(dist, dist);
	float r3 = r2 * glm::sqrt(r2 + 0.25f * 0.25f);
	//Calculating force
	float F = (other.w * G) / (r3 + 0.01f);
	//Acceleration
	mAcc += F * dist;
}

void Body::resetForce()
{
	mAcc = { 0.f };
}

void Body::reset()
{
	mPos = { 0.f };
	mAcc = { 0.f };
	mVel = { 0.f };
}

bool Body::operator != (Body & b)
{
	if (mMass != b.mMass)
	{
		return true;
	}
	else
	{
		return false;
	}
}