#include "PhysicSystem.h"
#include "ECS/MotionComponent.h"
#include "ECS/TransformComponent.h"

PhysicSystem::PhysicSystem()
{
}

PhysicSystem::~PhysicSystem()
{
}

void PhysicSystem::Update(float dt)
{
	for (Entity e : entities)
	{
		auto& motion = coordinator->GetComponent<MotionComponent>(e);
		auto& transform = coordinator->GetComponent<TransformComponent>(e);

		transform.Translate(motion.GetVelocity() * dt);
	}
}