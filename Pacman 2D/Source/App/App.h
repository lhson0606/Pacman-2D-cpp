#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

class App
{
public:
	App();
	int Run();
	~App();
private:
	int w = 800;
	int h = 600;
	const char* title = "We are sure to drown";
	GLFWwindow* window = nullptr;
	bool isRunning = false;

	int Init();
	void Draw();
	void Update(float dt);
};

