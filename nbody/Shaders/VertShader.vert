#version 330 core
layout (location = 0) in vec3 cubeVert;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inPos;
layout (location = 3) in float inMass;

float scale = 0.005f;

out float Mass;
out vec3 normal;
out vec3 fragPos;

uniform mat4 MVP;

void main()
{
	vec3 actualPos = inPos + cubeVert * inMass * scale;
	fragPos = actualPos;
	gl_Position = MVP * vec4(actualPos, 1.0); 
	Mass = inMass;
	normal = inNormal * inMass * scale;
}