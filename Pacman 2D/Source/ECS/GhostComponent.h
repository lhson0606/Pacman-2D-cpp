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

	//the mode is always NOT_ACTIVE when the State is not ACTIVE
	//and the ghost can be at one of three modes: CHASE, SCATTER, FRIGHTENED when the State is ACTIVE
	enum Mode
	{
		CHASE,//chase the player
		SCATTER,//go to a default position
		FRIGHTENED,//pick a random direction and move
		NOT_ACTIVE,//the ghost is at state DEAD || IN_CAGE || ACTIVE
	};

	enum State
	{
		ACTIVE,//when the ghost is alive, it can enter a mode
		DEAD,//the ghost is dead and need to go back to the ghost house for resurrection
		CAPTIVE,//used when the ghost is in the cage and cannot go out
		GOING_IN,//used to enable the ghost to move in/out the ghost house
		GOING_OUT,//used to enable the ghost to move in/out the ghost house
	};

	enum Direction
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		NONE_DIRECTION//at the start of the game, the ghost has no direction or there's no direction it can take
	};

	//the mode will determine the ghost behavior only when the state is ACTIVE
	int mode = NOT_ACTIVE;
	//the ghost state, can be ACTIVE, DEAD, CAPTIVE, GOING_OUT
	int state = CAPTIVE;
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
	//the current position that the ghost is aiming to reach in SCATTER mode
	glm::vec2 scatterPos = { 0,0 };
	//current ghost direction idx so that we can check if the ghost is going to change direction and update its velocity
	int dirIdx = NONE_DIRECTION;
	//the ghost needs this in order to update its direction only once per tile
	bool hasEnteredNewTile = true;
	glm::vec3 respawnPos = { 0,0,0 };

	GhostComponent();

	std::vector<glm::vec2> GetTargetPos() const;
	void ClearTargetPos();
	void PushbackTargetPos(const glm::vec2& pos);
	void EraseFirstTargetPos();
	glm::vec2 GetFirstTargetPos();
	bool IsTargetPosEmpty() const;

private:
	//position that the ghost need to reach
	std::vector<glm::vec2> targetPos;
};
