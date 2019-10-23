#version 330 core

out vec4 glFragColor;

in float width;

void main()
{
	vec3 color = vec3(width / width, width, width * width);
	glFragColor = vec4(color, 1.f);
}