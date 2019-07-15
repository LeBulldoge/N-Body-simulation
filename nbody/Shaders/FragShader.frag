#version 330 core

out vec4 gl_FragColor;
in float Mass;

void main()
{
	vec3 bodyColor = vec3(Mass); 
	vec3 diffuseColor = vec3(0.5f, 0.5f, 0.90f);

	vec3 color = bodyColor * diffuseColor;

	gl_FragColor = vec4(color, 1.f);
}