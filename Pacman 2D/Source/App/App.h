#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "ECS/ECS.h"
#include "Manager/ShaderManager.h"
#include "Manager/TextureManager.h"
#include <ECS/TileSystem.h>
#include <ECS/GhostSystem.h>
#include <ECS/DebugSystem.h>
#include <ECS/PhysicSystem.h>
#include <ECS/PlayerSystem.h>
#include <ECS/SharedData.h>
#include <ECS/KeyboardSystem.h>
#include "Render/Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class App
{
public:
	App();
	int Run();
	void Stop();
	~App();

	float w = 1024;
	float h = 960;
	float aspect = (float)w / h;

	dy::Camera cam;
	//glm::mat4 projection = glm::ortho(-w / h * glm::radians(fov), w / h * glm::radians(fov), -1.0f * glm::radians(fov), 1.0f * glm::radians(fov));
	glm::mat4 projection = glm::ortho(-w / h * glm::radians(cam.fov), w / h * glm::radians(cam.fov), -1.0f * glm::radians(cam.fov), 1.0f * glm::radians(cam.fov), 0.1f, 100.f);
	//glm::mat4 projection = glm::perspective(glm::radians(cam.fov), w / h, 0.1f, 100.0f);
	std::shared_ptr<Coordinator> coordinator = std::make_shared<Coordinator>();
	std::shared_ptr<TileSystem> tileSystem;
	std::shared_ptr<GhostSystem> ghostSystem;
	std::shared_ptr<DebugSystem> debugSystem;
	std::shared_ptr<PhysicSystem> physicSystem;
	std::shared_ptr<PlayerSystem> playerSystem;
	std::shared_ptr<KeyboardSystem> keyboardSystem;

	std::shared_ptr<SharedData> sharedData = std::make_shared<SharedData>();

	std::unique_ptr<ShaderManager> shaderManager = std::make_unique<ShaderManager>();
	std::unique_ptr<TextureManager> textureManager = std::make_unique<TextureManager>();

	void UpdateWindowView();
	void UpdateWindowProjection();
private:
	const char* title = "We are sure to drown";
	GLFWwindow* window = nullptr;
	bool isRunning = false;

	int Init();
	void InitSystems();
	void OnCreate();
	void Draw();
	void Update(float dt);
	void OnClose();
};
