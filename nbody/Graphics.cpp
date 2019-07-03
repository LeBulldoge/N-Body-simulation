#include "Graphics.h"

GLuint VBO;
GLuint VAO;
GLuint VBOcube;
GLuint VAOcube;

GLuint program;
GLFWwindow* window;

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

	/*if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		bodies = randBodies(BODIES);
	}*/

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

int initGLFW()
{
	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "N-Body simulation", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);

	return 0;
}

int initGLEW()
{
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		return -1;
	}

	return 0;
}

void initGFX()
{
	Shader shader;
	shader.source(GL_VERTEX_SHADER, "Shaders/VertShader.vert");
	shader.source(GL_FRAGMENT_SHADER, "Shaders/FragShader.frag");
	shader.link();
	program = shader.ID();

	float vertices[] = { -1,-1, -1,1, 1,1, 1,1, 1,-1, -1,-1 };

	glGenBuffers(1, &VBOcube);
	glBindBuffer(GL_ARRAY_BUFFER, VBOcube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAOcube);
	glBindVertexArray(VAOcube);

	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, AMOUNT * sizeof(Body), NULL, GL_STREAM_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Body), (void*)0);
	glVertexAttribDivisor(0, 1);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Body), (void*)sizeof(glm::vec3));
	glVertexAttribDivisor(1, 1);
	glEnableVertexAttribArray(1);

	glViewport(0, 0, WIDTH, HEIGHT);
	/*glm::mat4 proj(1.f);
	proj = glm::ortho(0, WIDTH, HEIGHT, 0, 0, 100);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, glm::value_ptr(proj));
	glMatrixMode(GL_PROJECTION);*/
	glLoadIdentity();
	glOrtho(0, WIDTH, HEIGHT, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
}

void drawBodies(GLfloat* bods)
{
	glClearColor(0.01f, 0.10f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, AMOUNT * sizeof(Body), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, AMOUNT * sizeof(Body), bods);

	/*glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(glm::vec3(0.f, 0.f, 10.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f));

	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));*/

	glBindVertexArray(VAO);
	glDrawArraysInstanced(GL_POINTS, 0, 1, AMOUNT);
	glBindVertexArray(0);
}