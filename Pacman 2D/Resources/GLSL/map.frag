#version 330 core

in vec2 textCoord;
out vec4 fragColor;

uniform sampler2D text1;


void main()
{
	fragColor = texture(text1, textCoord);
}