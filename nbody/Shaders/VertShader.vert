#version 330 core
layout (location = 0) in vec3 cubeVert;
layout (location = 1) in vec3 inPos;
layout (location = 2) in float inMass;
layout (location = 3) in vec2 inUV;

float scale = 0.6f;

out float Mass;
out vec2 UV;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_PointSize = 2.f;
	vec3 actualPos = inPos + cubeVert * 0.009f;
	gl_Position = vec4(actualPos, 1.0);
	Mass = inMass;
	UV = inUV;
}