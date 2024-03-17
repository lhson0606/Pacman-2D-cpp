#include "SharedData.h"

SharedData::SharedData()
{
}

glm::vec2 SharedData::GetPacmanPos() const
{
	return pacmanPos;
}

void SharedData::SetPacmanPos(const glm::vec2& pos)
{
	this->pacmanPos = pos;
}

int SharedData::GetPacmanDir() const
{
	return pacmanDir;
}

void SharedData::SetPacmanDir(int dir)
{
	this->pacmanDir = dir;
}

SharedData::~SharedData()
{
}