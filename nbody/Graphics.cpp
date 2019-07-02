#include "Graphics.h"

GLuint VBO;
GLuint VAO;
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

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, AMOUNT * sizeof(Body), NULL, GL_STREAM_DRAW);

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

	glPointSize(2);
}

void drawBodies(GLfloat* bods)
{
	glClearColor(0.01f, 0.10f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, AMOUNT * sizeof(Body), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, AMOUNT * sizeof(Body), bods);

	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, AMOUNT);
	glBindVertexArray(0);
}