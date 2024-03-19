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

bool SharedData::IsPathDebugEnabled() const
{
	return this->enablePathDebug;
}

void SharedData::EnablePathDebug(bool enable)
{
	this->enablePathDebug = enable;
}

bool* SharedData::GetPathDebugPtr()
{
	return &enablePathDebug;
}

bool SharedData::IsBtnChaseClicked() const
{
	return btnChaseClicked;
}

bool* SharedData::GetBtnChasePtr()
{
	return &btnChaseClicked;
}

bool SharedData::IsBtnScatterClicked() const
{
	return btnScatterClicked;
}

bool* SharedData::GetBtnScatterPtr()
{
	return &btnScatterClicked;
}

SharedData::~SharedData()
{
}