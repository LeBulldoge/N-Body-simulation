#include "Graphics.h"

GLuint VBO;
GLuint VAO;
GLuint VBOshape;
GLuint VAOshape;
GLuint IBOshape;

GLuint VBObox;
GLuint VBOboxpos;
GLuint VAObox;
GLuint IBObox;

GLuint programBodies;
GLuint programBoxes;
GLFWwindow* window;

int frameBufferWidth = WIDTH;
int frameBufferHeight = HEIGHT;

void framebuffer_resize_callback(GLFWwindow* wnd, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
};

static void key_callback(GLFWwindow* wnd, int key, int scancode, int action, int mods)
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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "N-Body simulation", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

	//glfwSetKeyCallback(window, key_callback);

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

void initGFXBodies(const glm::mat4& MVP)
{
	Shader shader;
	shader.source(GL_VERTEX_SHADER, "Shaders/VertShader.vert");
	shader.source(GL_FRAGMENT_SHADER, "Shaders/FragShader.frag");
	shader.link();
	programBodies = shader.ID();
	glUseProgram(programBodies);

	const float X = 0.525731112119133606f;
	const float Y = 0.f;
	const float Z = 0.850650808352039932f;

	const float vertices[] =
	{
		-X,  Y,  Z,
		 X,  Y,  Z,
		-X,  Y, -Z,
		 X,  Y, -Z,
					  
		 Y,  Z,  X,
		 Y,  Z, -X,
		 Y, -Z,  X,
		 Y, -Z, -X,
					  
		 Z,  X,  Y,
		-Z,  X,  Y,
		 Z, -X,  Y,
		-Z, -X,  Y, 
	};

	const unsigned short elements[] = {
		 0,  4,  1, 
		 0,  9,  4, 
		 9,  5,  4, 
		 4,  5,  8, 
		 4,  8,  1, 
		 8, 10,  1, 
		 8,  3, 10, 
		 5,  3,  8, 
		 5,  2,  3, 
		 2,  7,  3, 
		 7, 10,  3, 
		 7,  6, 10, 
		 7, 11,  6, 
		 11, 0,  6, 
		 0,  1,  6, 
		 6,  1, 10, 
		 9,  0, 11, 
		 9, 11,  2, 
		 9,  2,  5, 
		 7,  2, 11
	};

	glGenBuffers(1, &VBOshape);
	glBindBuffer(GL_ARRAY_BUFFER, VBOshape);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAOshape);
	glBindVertexArray(VAOshape);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);

	glGenBuffers(1, &IBOshape);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOshape);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), &elements, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, AMOUNT * sizeof(Body), nullptr, GL_STREAM_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Body), nullptr);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Body), (void*)sizeof(glm::vec3));

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);

	glViewport(0, 0, WIDTH, HEIGHT);
	glUniformMatrix4fv(glGetUniformLocation(programBodies, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glUseProgram(0);
}

void initGFXBoxes(const glm::mat4& MVP, const int amount)
{
	Shader shader;
	shader.source(GL_VERTEX_SHADER, "Shaders/BoxVertShader.vert");
	shader.source(GL_FRAGMENT_SHADER, "Shaders/BoxFragShader.frag");
	shader.link();
	programBoxes = shader.ID();
	glUseProgram(programBoxes);

	const float boxVertices[] =
	{
		 1.f,  1.f,  1.f,
		 1.f,  1.f, -1.f,
		-1.f,  1.f, -1.f,
		-1.f,  1.f,  1.f,

		 1.f, -1.f,  1.f,
		 1.f, -1.f, -1.f,
		-1.f, -1.f, -1.f,
		-1.f, -1.f,  1.f
	};

	const unsigned short boxElements[] = {
		 0, 4,
		 0, 1,
		 1, 5,
		 1, 2,
		 2, 6,
		 2, 3,
		 3, 7,
		 3, 0,
		 4, 5,
		 5, 6,
		 6, 7,
		 7, 4
	};

	glGenBuffers(1, &VBObox);
	glBindBuffer(GL_ARRAY_BUFFER, VBObox);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), &boxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);

	glGenBuffers(1, &VBOboxpos);
	glBindBuffer(GL_ARRAY_BUFFER, VBOboxpos);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(BoundingBox), nullptr, GL_STREAM_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(BoundingBox), nullptr);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(BoundingBox), (void*)sizeof(glm::vec3));

	glGenVertexArrays(1, &VAObox);
	glBindVertexArray(VAObox);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);;

	glGenBuffers(1, &IBObox);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBObox);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boxElements), &boxElements, GL_STATIC_DRAW);

	glUniformMatrix4fv(glGetUniformLocation(programBoxes, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

	glUseProgram(0);
}

void drawBodies(const Body* bods, const glm::mat4& MVP)
{
	glClearColor(0.01f, 0.03f, 0.08f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(programBodies);

	glUniformMatrix4fv(glGetUniformLocation(programBodies, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBOshape);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);

	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, AMOUNT * sizeof(Body), nullptr, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, AMOUNT * sizeof(Body), bods);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Body), nullptr);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Body), (void*)sizeof(glm::vec3));

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOshape);
	int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElementsInstanced(GL_TRIANGLES, size / sizeof(unsigned short), GL_UNSIGNED_SHORT, nullptr, AMOUNT);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glUseProgram(0);
}

void drawBox(const BoundingBox* boxes, const glm::mat4& MVP, const int amount)
{
	//glClearColor(0.01f, 0.03f, 0.08f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(programBoxes);

	glUniformMatrix4fv(glGetUniformLocation(programBoxes, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBObox);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, VBOboxpos);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(BoundingBox), nullptr, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, amount * sizeof(BoundingBox), boxes);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(BoundingBox), nullptr);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(BoundingBox), (void*)sizeof(glm::vec3));

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBObox);
	int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElementsInstanced(GL_LINES, size / sizeof(unsigned short), GL_UNSIGNED_SHORT, nullptr, amount);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glUseProgram(0);
}