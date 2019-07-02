#include <iostream>
#include <random>
#include <vector>

#include <glm.hpp>
#include <vec3.hpp>

#include "Constants.h"
#include "Body.h"
#include "Graphics.h"

extern bool pause = false;

std::random_device seed;
std::mt19937 rng(seed());
std::uniform_real_distribution<> dis(0, 1);

glm::vec3 randomPos()
{
	glm::vec3 pos;
	float t = dis(rng) * 2 * PI;
	float s = dis(rng);
	pos.x = cos(t)*s;
	pos.y = sin(t)*s;
	pos.z = dis(rng);

	return pos;
}

glm::vec3 randomVel(glm::vec3 pos)
{
	// Initial velocity is 'orbital' velocity from position
	glm::vec3 vel = glm::cross(glm::vec3(pos), glm::vec3(0, 0, 1));
	float orbital_vel = sqrt(2 * glm::length(vel));
	vel = glm::normalize(vel)*orbital_vel;
	return vel;
}

std::vector<Body> randBodies(const int size)
{
	std::vector<Body> bods;
	//bods.push_back(
	//	Body(glm::vec3(0, 0, 0),
	//		glm::vec3(dis(rng), dis(rng), dis(rng)),
	//		glm::vec3(0, 0, 0),
	//		1e60
	//	));
	for (int i = 0; i < size; i++)
	{
		glm::vec3 pos = randomPos();
		glm::vec3 vel = randomVel(pos);
		bods.push_back(
			Body(pos,
				glm::vec3(dis(rng), dis(rng), dis(rng)),
				vel / 100000.f,
				dis(rng)
			)
		);
	}
	return bods;
}

int main(void)
{

	double lastTime = glfwGetTime();
	int nbFrames = 0;

	initGLFW();
	initGLEW();
	initGFX();

	std::vector<Body> bodies = randBodies(BODIES);

	while (!glfwWindowShouldClose(window))
	{

		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0)
		{
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		if (!pause)
		{
			for (auto& body : bodies)
			{
				body.resetG();
				if (glm::distance(body.pos, glm::vec3(0, 0, 0)) > 10)
				{
					body.pos = randomPos();
					body.vel = randomVel(body.pos) / 100000.f;
				}
				for (auto& other : bodies)
				{
					if (body != other)
					{
						body.addG(other);
					}
				}
			}

			for (auto& body : bodies)
			{
				body.update();
			}

			drawBodies((GLfloat*)&bodies[0]);

		}

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
