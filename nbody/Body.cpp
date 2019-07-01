#include "Body.h"

Body::Body(glm::vec3 p,
	glm::vec3 c,
	glm::vec3 v,
	const double mass)
	: pos(p*1000.f)
	, color(c)
	, vel(v*1000.f)
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
	glm::vec3 dist;
	dist = b.pos - pos;
	//Normalized distance
	float nDist = glm::distance(pos, b.pos) + 0.25;
	//Calculating force
	float F = ((b.mass * G) / (nDist * nDist * nDist));
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