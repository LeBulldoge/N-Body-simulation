#include "Body.h"
#include "Constants.h"


Body::Body() noexcept : Body({0.f, 0.f, 0.f}, { 0.f, 0.f, 0.f }, 0.f)
{

}


Body::Body(glm::vec3 pos, glm::vec3 vel , float mass) : mPos(pos), mVel(vel), mMass(mass)
{
	mAcc = { 0.f, 0.f, 0.f };
}

Body::~Body()
{

}

float Body::getMass() noexcept
{
	return mMass;
}
glm::vec3 Body::getAcc() noexcept
{
	return mAcc;
}
glm::vec3 Body::getVel() noexcept
{
	return mVel;
}
glm::vec3 Body::getPos() noexcept
{
	return mPos;
}

void Body::update() noexcept
{
	//Velocity
	mVel += mAcc * step;
	//Position
	mPos += mVel * step;
}

void Body::addForce(const glm::vec4& other)
{
	//Distance vector
	const glm::vec3 dist = glm::vec3(other) - mPos;
	//Normalized distance
	const float r2 = glm::dot(dist, dist);
	const float r3 = r2 * glm::sqrt(r2 + 0.25f * 0.25f);
	//Calculating force
	const float F = (other.w * static_cast<float>(G)) / (r3 + 0.01f);
	//Acceleration
	mAcc += F * dist;
}

void Body::resetForce() noexcept
{
	mAcc = { 0.f };
}

void Body::reset() noexcept
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

bool Body::operator != (float& bMass)
{
	if (mMass != bMass)
	{
		return true;
	}
	else
	{
		return false;
	}
}