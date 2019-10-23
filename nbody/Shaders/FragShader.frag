#version 330 core

out vec4 glFragColor;
in float Mass;

void main()
{
	vec3 bodyColor = vec3(Mass); 
	vec3 diffuseColor = vec3(0.5f, 0.5f, 0.90f);

	vec3 color = bodyColor * diffuseColor;

	glFragColor = vec4(color, 1.f);
}