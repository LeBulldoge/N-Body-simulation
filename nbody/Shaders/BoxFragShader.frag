#version 330 core

out vec4 gl_FragColor;

in float width;

void main()
{
	vec3 color = {1 - width, 1 - width, 1 - width};
	gl_FragColor = vec4(color, 1.f);
}