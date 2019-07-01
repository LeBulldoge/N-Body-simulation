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

std::vector<Body> bodies =
{
	// name,
	// pos,
	// vel,
	// mass, gm
	Body(glm::vec3(4.321102017786880E5,  7.332029220261886E5, -2.173368199099370E4),
		glm::normalize(glm::vec3(253,166,0)),
		glm::vec3(-7.189825590990792E-3, 1.078719625403305E-2, 1.611861005709897E-4),
		1.98892e30),
	Body(glm::vec3(4.827977920456001E7, -3.593848175336351E7, -7.407821383784354E6),
		glm::normalize(glm::vec3(193,193,193)),
		glm::vec3(2.001998825524805E1,  4.093878958529241E1,  1.507185201945884E0),
		0.33011e24),
	Body(glm::vec3(3.081230776389965E7, -1.037662195648798E8, -3.208271884613059E6),
		glm::normalize(glm::vec3(193,68,14)),
		glm::vec3(3.338454368178189E1,  9.667951253108880E0, -1.794288490452711E0),
		4.8685e24),
	Body(glm::vec3(-5.969534792469550E7, -1.384328330091216E8, -1.539442552493513E4),
		glm::normalize(glm::vec3(14,68,200)),
		glm::vec3(2.686684747512509E1, -1.191462405401140E1, -1.789306645543220E-4),
		5.97219e24),
	Body(glm::vec3(-2.457432353017395E7,  2.363419494094334E8,  5.529273093965277E6),
		glm::normalize(glm::vec3(193,68,14)),
		glm::vec3(-2.318343075686108E1, -4.864803362557868E-1, 5.585276593733890E-1),
		6.4185e23),
	Body(glm::vec3(-7.569735834852062E8, -3.021406720368661E8,  1.818386330099623E7),
		glm::normalize(glm::vec3(193,68,14)),
		glm::vec3(4.691818986427708E0, -1.151682040045604E1, -5.715174720721627E-2),
		1898.19e24),
	Body(glm::vec3(-1.644374719486625E8, -1.494399735641233E9,  3.252789329873234E7),
		glm::normalize(glm::vec3(193,68,14)),
		glm::vec3(9.071782458885481E0, -1.088556624490897E0, -3.415607795777443E-1),
		568.34e24),
	Body(glm::vec3(2.708324010627307E9,  1.246406855535664E9, -3.045762012091076E7),
		glm::normalize(glm::vec3(193,68,14)),
		glm::vec3(-2.896814088567216E0,  5.868838238458174E0,  5.925130485071195E-2),
		86.8103e24),
	Body(glm::vec3(4.260892227618985E9, -1.383625744199709E9, -6.970338029718751E7),
		glm::normalize(glm::vec3(193,68,14)),
		glm::vec3(1.643322631738114E0,  5.202138396781843E0, -1.453302666708880E-1),
		102.41e24),
	Body(glm::vec3(1.512995164271581E9, -4.750856430852462E9, 7.072265548052478E7),
		glm::normalize(glm::vec3(193,68,14)),
		glm::vec3(5.291385672638518E0, 5.025608122645461E-1, -1.605961044129924E0),
		0.01303e24),
	Body(glm::vec3(1.171404652999866E8, 3.898786439553194E8, -9.285654429283202E6),
		glm::normalize(glm::vec3(193,68,14)),
		glm::vec3(-1.749907309328118E1, 3.939248912132300E0, 3.348447503365496E0),
		939300e15),
	Body(glm::vec3(4.291592316654775E8, 4.130737867650583E7, -6.396862972703221E7),
		glm::normalize(glm::vec3(193,68,14)),
		glm::vec3(-6.437734248047895E0, 1.322396929216813E1, -8.591155928319642E0),
		205000e15),
	Body(glm::vec3(3.857026210081980E7, -4.565575631972048E+08, 1.022702769186957E8),
		glm::normalize(glm::vec3(193,68,14)),
		glm::vec3(1.466155363544588E1, 3.856604107629098E0, -1.468972870833428E0),
		20000e15),
	Body(glm::vec3(-3.096651822378528E8, 1.773993080224814E8, 3.240229550527219E7),
		glm::normalize(glm::vec3(193,68,14)),
		glm::vec3(-7.810311925207990E0, -1.736557473811398E1, 1.470044682760689E0),
		259000e15)
};

std::random_device seed;
std::mt19937 rng(seed());
std::uniform_real_distribution<> dis(0, 1);
std::uniform_real_distribution<> disMass(1, 2e30);

glm::vec3 randomParticlePos()
{
	// Random position on a 'thick disk'
	glm::vec3 particle;
	float t = dis(rng) * 2 * PI;
	float s = dis(rng) * 100000;
	particle.x = cos(t)*s;
	particle.y = sin(t)*s;
	particle.z = dis(rng) * 4;

	return particle;
}

glm::vec3 randomParticleVel(glm::vec3 pos)
{
	// Initial velocity is 'orbital' velocity from position
	glm::vec3 vel = glm::cross(glm::vec3(pos), glm::vec3(0, 0, 1));
	float orbital_vel = sqrt(2.0*glm::length(vel));
	vel = glm::normalize(vel)*orbital_vel;
	return vel;
}

std::vector<Body> randBodies(const int size)
{
	std::vector<Body> bods;
	for (int i = 0; i < size; i++)
	{
		glm::vec3 pos = randomParticlePos();
		glm::vec3 vel = randomParticleVel(pos);
		double bodyMass = pow(disMass(rng), glm::distance(glm::vec3(0, 0, 0), glm::abs(pos)) / 5);
		bods.push_back(
			Body(pos,
				glm::vec3(dis(rng), dis(rng), dis(rng)),
				vel,
				disMass(rng)
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
		bodies = randBodies(POINTS);
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
	window = glfwCreateWindow(WIDTH, HEIGHT, "N-body", NULL, NULL);
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

	bodies = randBodies(POINTS);

	Body sun = Body(glm::vec3(0, 0, 0),
		glm::normalize(glm::vec3(253, 166, 0)),
		glm::vec3(0, 0, 0),
		1.98892e31);

	Shader shader;
	shader.source(GL_VERTEX_SHADER, "Shaders/VertShader.glsl");
	shader.source(GL_FRAGMENT_SHADER, "Shaders/FragShader.glsl");
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Body), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Body), (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(1);

	glViewport(0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WIDTH, HEIGHT, 0, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(5);

	double lastTime = glfwGetTime();
	int nbFrames = 0;

	while (!glfwWindowShouldClose(window))
	{

		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0)
		{ // If last prinf() was more than 1 sec ago
// printf and reset timer
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		if (!pause)
		{
			for (auto& body : bodies)
			{
				body.resetG();
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
			glDrawArrays(GL_POINTS, 0, POINTS);
			glBindVertexArray(0);
		}

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
