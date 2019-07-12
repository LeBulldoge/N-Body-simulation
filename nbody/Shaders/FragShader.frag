#version 330 core

in vec2 UV;

out vec4 gl_FragColor;
in float Mass;
in vec3 normal;
in vec3 fragPos;

void main()
{
	vec3 bodyColor = vec3(Mass); 
	vec3 diffuseColor = vec3(0.5f, 0.2f, 0.1f);
	vec3 ambient = vec3(0.1f, 0.05f, 0.01f);

	vec3 norm = normalize(normal);
	vec3 lightDir = vec3(0.f, 0.f, 0.f) - fragPos;
	float brightness = dot(norm, lightDir) / (length(lightDir) * length(norm));
	brightness = clamp(brightness, 0, 1);

	vec3 color = (ambient + brightness * diffuseColor) * bodyColor;

	gl_FragColor = vec4(color, 1.f);
}