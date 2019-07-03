#version 330 core

out vec4 gl_FragColor;
in float outMass;

void main()
{
	gl_FragColor = vec4(outMass, outMass, outMass, 1.0);
}