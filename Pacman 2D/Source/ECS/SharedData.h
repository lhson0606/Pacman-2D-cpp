#pragma once

#include <glm/glm.hpp>

class SharedData
{
public:
	SharedData();
	glm::ivec2 GetPacmanTilePos() const;
	void SetPacmanTilePos(const glm::ivec2& pos);
	~SharedData();
private:
	glm::ivec2 pacmanTilePos = glm::ivec2(-1);
};
