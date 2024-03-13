#version 330 core

in vec2 texCoord;
in vec3 ghostColor;

out vec4 fragColor;

uniform sampler2D texture1;

void main()
{
	fragColor = texture(texture1, texCoord)*vec4(ghostColor, 1);
}