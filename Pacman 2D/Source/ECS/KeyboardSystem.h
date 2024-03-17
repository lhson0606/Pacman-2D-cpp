#pragma once

#include "ECS/ECS.h"

class KeyboardSystem : public System
{
public:
	KeyboardSystem();
	void PrepareAll();
	void SetCoordinator(std::shared_ptr<Coordinator> coordinator);
	void SetKeyPressed(int key);
	~KeyboardSystem();
private:
	std::shared_ptr<Coordinator> coordinator;
};
