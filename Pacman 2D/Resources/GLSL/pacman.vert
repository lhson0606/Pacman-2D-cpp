#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexcoords;

out vec2 texCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform int texRow;
uniform float rowStride;

void main()
{
	gl_Position = projection*view*model*vec4(aPos.xy, 0.0, 1.0);
	texCoords.x = texRow*rowStride + aTexcoords.x;
	texCoords.y = aTexcoords.y;
}