#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Constants.h"
#include "Body.h"

extern GLFWwindow* window;
extern bool pause;

void window_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
int initGLFW();
int initGLEW();
void initGFX();
void drawBodies(GLfloat* bods);