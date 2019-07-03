#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in float inMass;

float scale = 0.6f;

out float outMass;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_PointSize = 2.5f;
	gl_Position = vec4(inPos, 1.0);
	outMass = inMass;
}