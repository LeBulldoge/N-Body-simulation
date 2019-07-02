#include <iostream>
#include <random>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <vec3.hpp>

#include "Constants.h"
#include "Body.h"
#include "Shader.h"

bool pause = false;

std::vector<Body> bodies;

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

void window_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		bodies = randBodies(BODIES);
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{

		if (pause)
		{
			pause = false;
		}
		else
		{
			pause = true;
		}
	}
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}

int main(void)
{
	glewExperimental = true;
	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	window = glfwCreateWindow(WIDTH, HEIGHT, "N-Body simulation", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		return -1;
	}

	bodies = randBodies(BODIES);

	Shader shader;
	shader.source(GL_VERTEX_SHADER, "Shaders/VertShader.vert");
	shader.source(GL_FRAGMENT_SHADER, "Shaders/FragShader.frag");
	shader.link();

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, bodies.size() * sizeof(Body), &bodies[0], GL_DYNAMIC_DRAW);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Body), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Body), (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(1);

	glViewport(0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WIDTH, HEIGHT, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPointSize(3);

	double lastTime = glfwGetTime();
	int nbFrames = 0;

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
			int i = 0;
			for (auto& body : bodies)
			{
				body.update();
			}

			glClearColor(0.01f, 0.10f, 0.15f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glUseProgram(shader.ID());

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, bodies.size() * sizeof(Body), &bodies[0], GL_DYNAMIC_DRAW);

			glBindVertexArray(VAO);
			glDrawArrays(GL_POINTS, 0, BODIES);
			glBindVertexArray(0);
		}

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
