#version 330 core

#define GHOST_NUMBER 4

struct GhostData
{
	int body;
	int eye;
	vec3 color;
	mat4 model;
};

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in float aGhostId;
layout (location = 3) in float partId;

out vec2 texCoord;
out vec3 ghostColor;

uniform mat4 projection;
uniform mat4 view;

uniform float texRowStride;
uniform float texColStride;
uniform GhostData ghosts[GHOST_NUMBER];

void main()
{

	int iGhostId = int(aGhostId);
	int iPartId = int(partId);
	gl_Position = projection*view*ghosts[iGhostId].model*vec4(aPos.xy, 0.0, 1.0);	

	if(iPartId == 0)
    {
		texCoord.x = aTexCoords.x + texRowStride*ghosts[iGhostId].body;
	    texCoord.y = aTexCoords.y + texColStride*ghosts[iGhostId].body;
		ghostColor = ghosts[iGhostId].color;
	}
	else
    {
		texCoord.x = aTexCoords.x + texRowStride*ghosts[iGhostId].eye;
	    texCoord.y = aTexCoords.y + texColStride*ghosts[iGhostId].eye;
        ghostColor = vec3(1.0, 1.0, 1.0);
    }
}