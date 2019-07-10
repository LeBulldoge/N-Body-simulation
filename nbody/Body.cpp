#include "Body.h"

std::random_device Body::seed;
std::mt19937 Body::rng(seed());
std::uniform_real_distribution<> Body::dis(-1, 1);
std::uniform_real_distribution<> Body::disMass(0.1, 1);

glm::vec3 randomPos()
{
	glm::vec3 pos;
	float t = Body::dis(Body::rng) * 2.f * PI;
	float s = Body::dis(Body::rng);
	pos.x = cos(t)*s;
	pos.y = sin(t)*s;
	pos.z = Body::dis(Body::rng);

	return pos;
}

glm::vec3 randomVel(glm::vec3 pos)
{
	// Initial velocity is 'orbital' velocity from position
	glm::vec3 vel = glm::cross(glm::vec3(pos), glm::vec3(0, 0, 1));
	float orbital_vel = sqrt(2 * glm::length(vel));
	vel = glm::normalize(vel)*orbital_vel / 100000.f;
	return vel;
}

Body::Body()
{
	pos = randomPos();
	vel = randomVel(pos);
	acc = glm::vec3(0, 0, 0);
	mass = disMass(rng);
}


Body::Body(glm::vec3 p,
	glm::vec3 v,
	float mass)
	: pos(p)
	, vel(v)
	, acc(0, 0, 0)
	, mass(mass)
{
}

Body::~Body()
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

void Body::addG(const glm::vec4& other)
{
	//Distance vector
	glm::vec3 dist = glm::vec3(other.x, other.y, other.z) - pos;
	//Normalized distance
	float r2 = glm::dot(dist, dist);
	float r3 = r2 * glm::sqrt(r2 + 0.25f * 0.25f);
	//Calculating force
	float F = (other.w * G) / (r3 + 0.01f);
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