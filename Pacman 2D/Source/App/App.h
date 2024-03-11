#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "ECS/ECS.h"
#include "Render/ShaderManager.h"
#include <ECS/TileSystem.h>

class App
{
public:
	App();
	int Run();
	void Stop();
	~App();

private:
	int w = 800;
	int h = 600;
	const char* title = "We are sure to drown";
	GLFWwindow* window = nullptr;
	bool isRunning = false;

	int Init();
	void OnCreate();
	void Draw();
	void Update(float dt);
	void OnClose();
	
	std::shared_ptr<Coordinator> coordinator = std::make_shared<Coordinator>();
	std::shared_ptr<TileSystem> tileSystem;
	std::unique_ptr<ShaderManager> shaderManager = std::make_unique<ShaderManager>();;
};
