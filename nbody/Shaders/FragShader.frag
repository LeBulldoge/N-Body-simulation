#version 330 core

out vec4 gl_FragColor;
in vec3 outColor;

void main()
{
	gl_FragColor = vec4(outColor, 1.0);
}