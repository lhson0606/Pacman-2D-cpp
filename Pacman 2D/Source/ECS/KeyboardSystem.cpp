#include "KeyboardSystem.h"
#include "ECS/KeyboardComponent.h"

KeyboardSystem::KeyboardSystem()
{
}

void KeyboardSystem::PrepareAll()
{
	for (Entity e : entities)
	{
		coordinator->GetComponent<KeyboardComponent>(e).Prepare();
	}
}

void KeyboardSystem::SetCoordinator(std::shared_ptr<Coordinator> coordinator)
{
	this->coordinator = coordinator;
}

void KeyboardSystem::SetKeyPressed(int key)
{
	for (Entity e : entities)
	{
		coordinator->GetComponent<KeyboardComponent>(e).SetKeyPressed(key, true);
	}
}

KeyboardSystem::~KeyboardSystem()
{
}