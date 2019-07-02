#pragma once
#include <GL/glew.h>
#include <string>

class Shader {

public:
	GLuint id;
	
	Shader();

	void source(GLenum type, const std::string& file);

	void link();

	GLuint ID();

};