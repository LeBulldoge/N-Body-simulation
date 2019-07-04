#version 330 core

in vec2 UV;

out vec4 gl_FragColor;
in float Mass;

uniform sampler2D bodyTexture;

void main()
{
	gl_FragColor = texture2D(bodyTexture, UV) * vec4(Mass, Mass, Mass, 1.f);
}