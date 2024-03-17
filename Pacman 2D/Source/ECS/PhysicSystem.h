#pragma once

#include "ECS/ECS.h"

class PhysicSystem : public System
{
public:
	PhysicSystem();
	~PhysicSystem();

	void SetCoordinator(std::shared_ptr<Coordinator> coordinator) { this->coordinator = coordinator; }

	void Update(float dt);

	std::shared_ptr<Coordinator> coordinator;
};
