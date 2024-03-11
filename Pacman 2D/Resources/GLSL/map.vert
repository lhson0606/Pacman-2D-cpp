#version 330 core

layout (location = 0) in vec2 aPos;
layout(location = 1) in int id;

out vec2 textCoord;

uniform mat4 model[1000];
uniform int textIndex[1000];
uniform float scale;

uniform int textWidth;
uniform int textHeight;

void main()
{
	gl_Position = model[id] * vec4(aPos.xy, 0.0, 1.0) * scale;
	textCoord.x = textIndex[id] % textWidth / textWidth;
	textCoord.y = textIndex[id] / textHeight / textHeight;
}