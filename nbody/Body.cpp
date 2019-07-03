#include "Body.h"

Body::Body(glm::vec3 p,
	glm::vec3 v,
	float mass)
	: pos(p)
	, vel(v)
	, acc(0, 0, 0)
	, mass(mass)
{
}

double Body::Mass()
{
	return mass;
}
glm::vec3 Body::Acc()
{
	return acc;
}
glm::vec3 Body::Vel()
{
	return vel;
}
glm::vec3 Body::Pos()
{
	return pos;
}

void Body::update()
{
	//Velocity
	vel += acc * step;
	//Position
	pos += vel * step;
}

void Body::addG(Body& b)
{
	//Distance vector
	glm::vec3 dist = b.pos - pos;
	//Normalized distance
	float r2 = glm::dot(dist, dist);
	float r3 = r2 * glm::sqrt(r2 + 0.25f * 0.25f);
	//Calculating force
	float F = ((b.mass * G) / (r3 + 0.01f));
	//Acceleration
	acc += F * dist;
}

void Body::resetG()
{
	acc.x = 0.0;
	acc.y = 0.0;
	acc.z = 0.0;
}

bool Body::operator != (Body & b)
{
	if (mass != b.mass)
	{
		return true;
	}
	else
	{
		return false;
	}
}