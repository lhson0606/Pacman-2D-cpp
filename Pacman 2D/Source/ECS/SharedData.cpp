#include "SharedData.h"

SharedData::SharedData()
{
}

glm::ivec2 SharedData::GetPacmanTilePos() const
{
	return pacmanTilePos;
}

void SharedData::SetPacmanTilePos(const glm::ivec2& pos)
{
	pacmanTilePos = pos;
}

SharedData::~SharedData()
{
}