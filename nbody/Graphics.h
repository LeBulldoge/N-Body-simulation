#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Shader.h"
#include "Constants.h"
#include "Body.h"
#include <iostream>

extern GLFWwindow* window;
extern bool pause;

void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
int initGLFW();
int initGLEW();
void initGFX(const glm::mat4& MVP);
void drawBodies(Body* bods, const glm::mat4& MVP);