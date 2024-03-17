#pragma once

#include <glm/glm.hpp>
#include <vector>

class GhostComponent
{
public:

	//Blinky (red), Pinky (pink), Inky (cyan), and Clyde (orange)
	//reference: https://www.color-hex.com/color-palette/10122

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

	int mode = SCATTER;
	int state = IN_CAGE;
	int part[2] = { 0, 0 };
	glm::vec3 color = { 1,1,1 };
	glm::vec3 startPos = { 0,0,0 };
	std::vector<glm::ivec2> path;
	int type = -1;
	glm::vec2 targetPos = { 0,0 };
	glm::vec2 scatterPos = { 0,0 };

	GhostComponent();
};
