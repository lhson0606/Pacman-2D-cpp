#version 330 core

#define MAX_COLOR_COUNT 4
#define MAX_TARGET_TILE 4

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexcoord;
layout (location = 2) in float id;

out vec2 texCoord;
out vec3 debugColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 models[MAX_TARGET_TILE];
uniform vec3 colors[MAX_COLOR_COUNT];

void main()
{
	gl_Position = projection*view*models[int(id)]*vec4(aPos.xy, 0.0, 1.0);
	debugColor = colors[int(id)];
	texCoord = aTexcoord;
}