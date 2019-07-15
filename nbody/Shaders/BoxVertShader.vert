#version 330 core
layout (location = 0) in vec3 boxVert;
layout (location = 1) in vec3 inPos;
layout (location = 2) in float inWidth;

float scale = 0.005f;

out float width;
uniform mat4 MVP;

void main()
{
	width = inWidth;
	vec3 actualPos = inPos + boxVert * inWidth;
	gl_Position = MVP * vec4(actualPos, 1.0); 
}