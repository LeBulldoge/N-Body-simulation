#version 330 core
layout (location = 0) in vec3 cubeVert;
layout (location = 1) in vec3 inPos;
layout (location = 2) in float inMass;

float scale = 0.002f;

out float Mass;

uniform mat4 MVP;

void main()
{
	vec3 actualPos = inPos + cubeVert * inMass * scale;
	gl_Position = MVP * vec4(actualPos, 1.0); 
	Mass = inMass;
}