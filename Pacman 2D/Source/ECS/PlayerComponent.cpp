#include "PlayerComponent.h"
#include "assert.h"

PlayerComponent::PlayerComponent()
{
	this->life = 1;
}

PlayerComponent::PlayerComponent(int initialLife)
{
	this->life = initialLife;
}

PlayerComponent::~PlayerComponent()
{
}

int PlayerComponent::GetLife()
{
	return life;
}

void PlayerComponent::DecreaseLife()
{
	life--;
}

void PlayerComponent::IncreaseLife()
{
	life++;
}

void PlayerComponent::ResetLife()
{
	life = 1;
}

bool PlayerComponent::IsEnergized()
{
	return this->isEnergized;
}

void PlayerComponent::SetEnergized(int duration)
{
	assert("Not implemented yet" && false);
}

int PlayerComponent::GetAnimIdx()
{
	return animIdx;
}

void PlayerComponent::SetAnimIdx(int idx)
{
	this->animIdx = idx;
}

bool PlayerComponent::HasEnteredNewTile()
{
	return hasEnteredNewTile;
}

void PlayerComponent::SetEnteredNewTile(bool val)
{
	this->hasEnteredNewTile = val;
}

PlayerComponent::MoveDirection PlayerComponent::GetInputDirection()
{
	return inpDir;
}

void PlayerComponent::SetInputDirection(MoveDirection dir)
{
	this->inpDir = dir;
}