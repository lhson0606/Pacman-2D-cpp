#pragma once

#include <glm/glm.hpp>

class SharedData
{
public:
	SharedData();
	glm::vec2 GetPacmanPos() const;
	void SetPacmanPos(const glm::vec2& pos);
	int GetPacmanDir() const;
	void SetPacmanDir(int dir);
	bool IsPathDebugEnabled() const;
	void EnablePathDebug(bool enable);
	bool* GetPathDebugPtr();
	bool IsBtnChaseClicked() const;
	bool* GetBtnChasePtr();
	bool IsBtnScatterClicked() const;
	bool* GetBtnScatterPtr();
	~SharedData();
private:
	glm::vec2 pacmanPos = glm::vec2(-1);
	int pacmanDir = 0;
	bool btnChaseClicked = false;
	bool btnScatterClicked = false;
	bool enablePathDebug = true;
};
