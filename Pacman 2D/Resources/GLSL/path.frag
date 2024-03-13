#version 330 core

out vec4 fragColor;

in vec3 debugColor;

void main()
{
	fragColor = vec4(debugColor, 0.2);
}