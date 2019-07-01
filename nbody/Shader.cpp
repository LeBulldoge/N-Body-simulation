#include "Shader.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

Shader::Shader() : id(0) {}

void Shader::source(GLenum type, const string &file)
{
	if (!id) id = glCreateProgram();

	string code;

	try
	{
		stringstream sstream;
		{
			ifstream stream;
			stream.exceptions(ifstream::failbit | ifstream::badbit);
			stream.open(file);
			sstream << stream.rdbuf();
		}
		code = sstream.str();
	}
	catch (ifstream::failure e)
	{
		throw std::runtime_error(std::string("Can't open ") + file + std::string(e.what()));
	}

	GLint success;
	GLchar info_log[2048];

	const char *s = code.c_str();

	GLuint shad_id = glCreateShader(type);
	glShaderSource(shad_id, 1, &s, NULL);
	glCompileShader(shad_id);
	glGetShaderiv(shad_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		// error log
		glGetShaderInfoLog(shad_id, sizeof(info_log), NULL, info_log);
		throw std::runtime_error(std::string("Can't compile ") + file + " " + info_log);
		exit(-1);
	}
	glAttachShader(id, shad_id);
}

void Shader::link()
{
	GLint success;
	GLchar info_log[2048];

	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, sizeof(info_log), NULL, info_log);
		throw std::runtime_error(std::string("Can't link ") + std::string(info_log));
	}
}

GLuint Shader::ID() { return id; }