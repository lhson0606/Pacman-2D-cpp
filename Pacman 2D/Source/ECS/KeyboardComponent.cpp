#include "KeyboardComponent.h"

KeyboardComponent::KeyboardComponent(std::string tag)
{
	this->tag = tag;
}

void KeyboardComponent::Prepare()
{
	keyPressedMap.reset();
}

bool KeyboardComponent::IsKeyPressed(int key)
{
	return keyPressedMap[key];
}

void KeyboardComponent::SetKeyPressed(int key, bool value)
{
	keyPressedMap[key] = value;
}

KeyboardComponent::~KeyboardComponent()
{
}