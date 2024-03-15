#version 330 core

#define MAX_COLOR_COUNT 4

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexcoord;
layout (location = 2) in float colorId;

out vec2 texcoord;
out vec3 debugColor;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 colors[MAX_COLOR_COUNT];

void main()
{
	gl_Position = projection*view*vec4(aPos.xy, 0.0, 1.0);
	debugColor = colors[int(colorId)];
	texcoord = aTexcoord;
}