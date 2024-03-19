#pragma once

#include <glm/glm.hpp>
#include <vector>

class GhostComponent
{
public:

	//Blinky (red), Pinky (pink), Inky (cyan), and Clyde (orange)
	//reference: https://www.color-hex.com/color-palette/10122

	//the colors divide by 255 to convert from 0-255 to 0-1 (OpenGL color scale)
	inline static const glm::vec3 PINKY_COLOR = { 234 / 255.0f,130 / 255.0f,229 / 255.0f };
	inline static const glm::vec3 INKY_COLOR = { 70 / 255.0f,191 / 255.0f,238 / 255.0f };
	inline static const glm::vec3 CLYDE_COLOR = { 219 / 255.0f,133 / 255.0f,28 / 255.0f };
	inline static const glm::vec3 BLINKY_COLOR = { 208 / 255.0f,62 / 255.0f,25 / 255.0f };

	enum Mode
	{
		CHASE,
		SCATTER,
		FRIGHTENED
	};

	enum State
	{
		DEAD,
		ALIVE,
		IN_CAGE
	};

	enum Direction
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		NONE
	};

	int mode = SCATTER;
	//the ghost state, can be DEAD, ALIVE, IN_CAGE
	int state = IN_CAGE;
	//part[0] = index to draw body and part[1] = index to draw eyes
	int part[2] = { 0, 0 };
	//ghost color
	glm::vec3 color = { 1,1,1 };
	//ghost start/resurrect position
	glm::vec3 startPos = { 0,0,0 };
	//used to draw debug path using AStart or Dijkstra
	std::vector<glm::ivec2> path;
	//ghost id to update in OpenglGL vertex buffer
	int type = -1;
	//the current position that the ghost is aiming to reach in CHASE mode
	glm::vec2 targetPos = { 0,0 };
	//the current position that the ghost is aiming to reach in SCATTER mode
	glm::vec2 scatterPos = { 0,0 };
	//current ghost direction idx so that we can check if the ghost is going to change direction and update its velocity
	int dirIdx = NONE;
	//the ghost needs this in order to update its direction only once per tile
	bool hasEnteredNewTile = true;

	GhostComponent();
};
