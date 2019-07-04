#include "Graphics.h"

GLuint VBO;
GLuint VAO;
GLuint VBOcube;
GLuint VAOcube;
GLuint texture;
GLuint sampler;

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

GLuint loadTexture(const char* imagePath)
{
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char* data;

	FILE* file;
	fopen_s(&file, imagePath, "rb");
	if (!file)
	{ 
		printf("File %s couldn't be opened!\n", imagePath); 
		return -1; 
	}
	if (fread(header, 1, 54, file) != 54)
	{
		printf("Not a correct BMP file!");
		return -1;
	}
	if (header[0] != 'B' || header[1] != 'M')
	{
		printf("Not a correct BMP file!");
		return -1;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	if (imageSize == 0)
	{
		imageSize = width * height * 3;
	}
	if (dataPos == 0)
	{
		dataPos = 54;
	}

	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	delete[] data;

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;
}

void initGFX()
{
	Shader shader;
	shader.source(GL_VERTEX_SHADER, "Shaders/VertShader.vert");
	shader.source(GL_FRAGMENT_SHADER, "Shaders/FragShader.frag");
	shader.link();
	program = shader.ID();

	sampler = glGetUniformLocation(program, "bodyTexture");

	float vertices[] = 
	{ 
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f, 0.5f, 0.0f, 
	};

	glGenBuffers(1, &VBOcube);
	glBindBuffer(GL_ARRAY_BUFFER, VBOcube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAOcube);
	glBindVertexArray(VAOcube);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, AMOUNT * sizeof(Body), NULL, GL_STREAM_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Body), (void*)0);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Body), (void*)sizeof(glm::vec3));

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);

	texture = loadTexture("texture.bmp");

	glViewport(0, 0, WIDTH, HEIGHT);

	glMatrixMode(GL_PROJECTION);
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
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBOcube);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, AMOUNT * sizeof(Body), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, AMOUNT * sizeof(Body), bods);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Body), (void*)0);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Body), (void*)sizeof(glm::vec3));

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, AMOUNT);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}