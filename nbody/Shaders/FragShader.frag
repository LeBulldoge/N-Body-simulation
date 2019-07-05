#version 330 core

in vec2 UV;

out vec4 gl_FragColor;
in float Mass;

uniform sampler2D bodyTexture;

void main()
{
	vec4 tex = texture2D(bodyTexture, UV);
	if (tex.a < 0.5)
		discard;
	gl_FragColor = tex * vec4(Mass, Mass, Mass, 1.f);
}