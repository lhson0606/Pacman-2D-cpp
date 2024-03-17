#pragma once

#include <string>

#include <GLFW/glfw3.h>
#include <bitset>

#define DY_ENGINE_KEY_COUNT 349

class KeyboardComponent
{
public:
	KeyboardComponent() = default;
	KeyboardComponent(std::string tag);
	void Prepare();
	bool IsKeyPressed(int key);
	void SetKeyPressed(int key, bool value);
	~KeyboardComponent();
	std::string getTag() { return tag; }
private:
	std::string tag = "<empty>";
	//using glfw key codes
	std::bitset<DY_ENGINE_KEY_COUNT> keyPressedMap{ 0 };
};
