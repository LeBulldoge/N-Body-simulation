#pragma once
#include <GL/glew.h>
#include <string>

class Shader {

	GLuint id;

public:
	
	Shader();

	void source(GLenum type, const std::string& file);

	void link();

	GLuint ID();

};