#version 330 core
layout (location = 0) in vec3 cubeVert;
layout (location = 1) in vec3 inPos;
layout (location = 2) in float inMass;

float scale = 0.05f;

out float Mass;
out vec2 UV;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_PointSize = 2.f;
	vec3 actualPos = inPos + cubeVert * (inMass * inPos.z * scale);
	gl_Position = vec4(actualPos, 1.0); 
	Mass = inMass;
	UV = cubeVert.xy + vec2(0.5, 0.5);
}