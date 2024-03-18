#include "App.h"

#include <iostream>
#include <ECS/ECS.h>
#include <Windows.h>
#include <Map/Map.h>

#include <ECS/SpriteComponent.h>
#include <ECS/TransformComponent.h>
#include <ECS/TileComponent.h>
#include <ECS/GhostComponent.h>
#include <ECS/DebugPathComponent.h>
#include <ECS/TilePositionComponent.h>
#include <ECS/MotionComponent.h>
#include <ECS/KeyboardComponent.h>
#include <ECS/PlayerComponent.h>
#include <chrono>

#include "dy/Log.h"

void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

App::App()
{
	window = nullptr;
}

int App::Run()
{
	if (Init() != 0)
	{
		return -1;
	}

	isRunning = true;

	OnCreate();

	auto startTime = std::chrono::high_resolution_clock::now();
	auto endTime = std::chrono::high_resolution_clock::now();

	float dt = 0.0f;

	while (!glfwWindowShouldClose(window) && this->isRunning)
	{
		startTime = std::chrono::high_resolution_clock::now();
		//process any input events
		processInput(window);

		//our rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		Draw();
		Update(dt);

		//swap buffer for the next frame to be drawn
		glfwSwapBuffers(window);
		//check for any events to happen (keyboard, mouse movement, etc)
		glfwPollEvents();
		startTime = endTime;
		endTime = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<float>(endTime - startTime).count();
	}

	OnClose();

	return 0;
}

void App::UpdateWindowView()
{
	tileSystem->LoadViewMat(shaderManager->GetShader(ShaderManager::ShaderType::MAP), cam.view);
	ghostSystem->LoadViewMat(shaderManager->GetShader(ShaderManager::ShaderType::GHOST), cam.view);
	debugSystem->LoadViewMat(
		shaderManager->GetShader(ShaderManager::ShaderType::DEBUG_PATH), 
		shaderManager->GetShader(ShaderManager::ShaderType::TARGET_TILE), 
		cam.view);
	playerSystem->LoadViewMat(shaderManager->GetShader(ShaderManager::ShaderType::PACMAN), cam.view);
}

void App::UpdateWindowProjection()
{
	tileSystem->LoadProjectMat(shaderManager->GetShader(ShaderManager::ShaderType::MAP), projection);
	ghostSystem->LoadProjectMat(shaderManager->GetShader(ShaderManager::ShaderType::GHOST), projection);
	debugSystem->LoadProjectMat(
		shaderManager->GetShader(ShaderManager::ShaderType::DEBUG_PATH),
		shaderManager->GetShader(ShaderManager::ShaderType::TARGET_TILE),
		projection);
	playerSystem->LoadProjectMat(shaderManager->GetShader(ShaderManager::ShaderType::PACMAN), projection);
}

int App::Init()
{
	glfwInit();
	//this will tell glfw prerequisite hints, if the conditions are not met, gflw will log errors and close the window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//for Mac OS X
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//create our window most glfw needs a window to work with
	this->window = glfwCreateWindow(w, h, this->title, NULL, NULL);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//make context for the current render thread
	glfwMakeContextCurrent(window);

	//before using any render function we need to call this once to retrieve all functions pointer
	//our program will result in error if it does't call this
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//set window resize callback
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetScrollCallback(window, mouseScrollCallback);

	//set a pointer to the current object, so that we can retrieve it in the callback function
	glfwSetWindowUserPointer(window, this);

	//set console color to white
	DyLogger::SetConsoleColor(DyLogger::CONSOLE_WHITE_COLOR);
	return 0;
}

void App::InitSystems()
{
	coordinator->RegisterComponent<TransformComponent>();
	coordinator->RegisterComponent<SpriteComponent>();
	coordinator->RegisterComponent<TileComponent>();
	coordinator->RegisterComponent<GhostComponent>();
	coordinator->RegisterComponent<DebugPathComponent>();
	coordinator->RegisterComponent<TilePositionComponent>();
	coordinator->RegisterComponent<MotionComponent>();
	coordinator->RegisterComponent<KeyboardComponent>();
	coordinator->RegisterComponent<PlayerComponent>();

	tileSystem = coordinator->RegisterSystem<TileSystem>();
	tileSystem->SetCoordinator(coordinator);
	Signature tileSystemSignature;
	tileSystemSignature.set(coordinator->GetComponentType<TransformComponent>());
	tileSystemSignature.set(coordinator->GetComponentType<TileComponent>());
	tileSystemSignature.set(coordinator->GetComponentType<TilePositionComponent>());
	coordinator->SetSystemSignature<TileSystem>(tileSystemSignature);

	ghostSystem = coordinator->RegisterSystem<GhostSystem>();
	ghostSystem->SetCoordinator(coordinator);
	ghostSystem->SetSharedData(sharedData);
	Signature ghostSystemSignature;
	ghostSystemSignature.set(coordinator->GetComponentType<TransformComponent>());
	ghostSystemSignature.set(coordinator->GetComponentType<GhostComponent>());
	ghostSystemSignature.set(coordinator->GetComponentType<TilePositionComponent>());
	ghostSystemSignature.set(coordinator->GetComponentType<DebugPathComponent>());
	coordinator->SetSystemSignature<GhostSystem>(ghostSystemSignature);

	debugSystem = coordinator->RegisterSystem<DebugSystem>();
	debugSystem->SetCoordinator(coordinator);
	Signature debugSystemSignature;
	debugSystemSignature.set(coordinator->GetComponentType<DebugPathComponent>());
	coordinator->SetSystemSignature<DebugSystem>(debugSystemSignature);

	physicSystem = coordinator->RegisterSystem<PhysicSystem>();
	physicSystem->SetCoordinator(coordinator);
	Signature physicSystemSignature;
	physicSystemSignature.set(coordinator->GetComponentType<TransformComponent>());
	physicSystemSignature.set(coordinator->GetComponentType<MotionComponent>());
	coordinator->SetSystemSignature<PhysicSystem>(physicSystemSignature);

	keyboardSystem = coordinator->RegisterSystem<KeyboardSystem>();
	keyboardSystem->SetCoordinator(coordinator);
	Signature keyboardSystemSignature;
	keyboardSystemSignature.set(coordinator->GetComponentType<KeyboardComponent>());
	coordinator->SetSystemSignature<KeyboardSystem>(keyboardSystemSignature);

	playerSystem = coordinator->RegisterSystem<PlayerSystem>();
	playerSystem->SetCoordinator(coordinator);
	playerSystem->SetSharedData(sharedData);
	Signature playerSystemSignature;
	playerSystemSignature.set(coordinator->GetComponentType<PlayerComponent>());
	coordinator->SetSystemSignature<PlayerSystem>(playerSystemSignature);

	//load the shaders
	shaderManager->HardLoadShaders();
	//load textures
	textureManager->HardLoadTextures();
}

void App::OnCreate()
{
	InitSystems();
	//load the map
	std::shared_ptr<Map> map = Map::LoadMap("Resources/Maps/level1.json");
	tileSystem->InitMap(map, cam);
	tileSystem->LoadProjectMat(shaderManager->GetShader(ShaderManager::ShaderType::MAP), projection);
	tileSystem->LoadViewMat(shaderManager->GetShader(ShaderManager::ShaderType::MAP), cam.view);
	tileSystem->LoadTexture(shaderManager->GetShader(ShaderManager::ShaderType::MAP), textureManager->GetTexture(TextureManager::TextureType::MAP));

	//load the ghost
	ghostSystem->Init(map);
	ghostSystem->LoadProjectMat(shaderManager->GetShader(ShaderManager::ShaderType::GHOST), projection);
	ghostSystem->LoadViewMat(shaderManager->GetShader(ShaderManager::ShaderType::GHOST), cam.view);
	ghostSystem->LoadTexture(shaderManager->GetShader(ShaderManager::ShaderType::GHOST), textureManager->GetTexture(TextureManager::TextureType::GHOST));
	ghostSystem->LoadExtra(shaderManager->GetShader(ShaderManager::ShaderType::GHOST));

	//init debug system
	debugSystem->Init();
	debugSystem->LoadProjectMat(
		shaderManager->GetShader(ShaderManager::ShaderType::DEBUG_PATH), 
		shaderManager->GetShader(ShaderManager::ShaderType::TARGET_TILE),
		projection);
	debugSystem->LoadViewMat(
		shaderManager->GetShader(ShaderManager::ShaderType::DEBUG_PATH), 
		shaderManager->GetShader(ShaderManager::ShaderType::TARGET_TILE),
		cam.view);
	debugSystem->LoadColors(
		shaderManager->GetShader(ShaderManager::ShaderType::DEBUG_PATH),
		shaderManager->GetShader(ShaderManager::ShaderType::TARGET_TILE),
		GhostComponent::BLINKY_COLOR,
		GhostComponent::PINKY_COLOR,
		GhostComponent::INKY_COLOR,
		GhostComponent::CLYDE_COLOR
	);
	debugSystem->LoadTexture(
		shaderManager->GetShader(ShaderManager::ShaderType::TARGET_TILE),
		textureManager->GetTexture(TextureManager::TextureType::TARGET_TILE)
	);

	playerSystem->Init(map);
	playerSystem->LoadProjectMat(shaderManager->GetShader(ShaderManager::ShaderType::PACMAN), projection);
	playerSystem->LoadViewMat(shaderManager->GetShader(ShaderManager::ShaderType::PACMAN), cam.view);
	playerSystem->LoadTexture(shaderManager->GetShader(ShaderManager::ShaderType::PACMAN), textureManager->GetTexture(TextureManager::TextureType::PACMAN));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	//print terrain debug string
	DyLogger::LogArg(DyLogger::LOG_INFO, "Terrain: %s", map->GetDebugString().c_str());
}

void App::Draw()
{
	playerSystem->Draw(
		shaderManager->GetShader(ShaderManager::ShaderType::PACMAN),
		textureManager->GetTexture(TextureManager::TextureType::PACMAN)
	);

	tileSystem->Draw(
		shaderManager->GetShader(ShaderManager::ShaderType::MAP),
		textureManager->GetTexture(TextureManager::TextureType::MAP)
	);

	if (true)
	{
		debugSystem->Draw(
			shaderManager->GetShader(ShaderManager::ShaderType::DEBUG_PATH),
			shaderManager->GetShader(ShaderManager::ShaderType::TARGET_TILE),
			textureManager->GetTexture(TextureManager::TextureType::TARGET_TILE)
		);
	}

	ghostSystem->Draw(
		shaderManager->GetShader(ShaderManager::ShaderType::GHOST),
		textureManager->GetTexture(TextureManager::TextureType::GHOST)
	);
}

void App::Update(float dt)
{
	ghostSystem->Update(dt);
	physicSystem->Update(dt);
	playerSystem->Update(dt);
}

void App::OnClose()
{
}

void App::Stop()
{
	isRunning = false;
}

App::~App()
{
	//release glfw resources before exiting
	glfwTerminate();
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	//this raw pointer is just a reference to the current object not the owner of the object

	App* app = (App*)glfwGetWindowUserPointer(window);
	app->aspect = (float)width / height;
	app->w = width;
	app->h = height;
	app->projection = glm::ortho(-app->w / app->h * glm::radians(app->cam.fov), app->w / app->h * glm::radians(app->cam.fov), -1.0f * glm::radians(app->cam.fov), 1.0f * glm::radians(app->cam.fov), 0.1f, 100.f);
	app->UpdateWindowProjection();
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	static double lastX = 0.0f;
	static double lastY = 0.0f;
	static bool firstMouse = true;

	App* app = (App*)glfwGetWindowUserPointer(window);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double dx = xpos - lastX;
	double dy = ypos - lastY;

	lastX = xpos;
	lastY = ypos;

	//app->cam.processMouseInput(dx, -dy);
	//app->tileSystem->LoadViewMat(app->shaderManager->GetShader(ShaderManager::ShaderType::MAP), app->cam.view);
}

void processInput(GLFWwindow* window)
{
	//this raw pointer is just a reference to the current object not the owner of the object
	App* app = (App*)glfwGetWindowUserPointer(window);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		app->Stop();
	}

	/*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		app->cam.processMove(dy::Camera::FORWARD);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		app->cam.processMove(dy::Camera::BACKWARD);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		app->cam.processMove(dy::Camera::LEFT);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		app->cam.processMove(dy::Camera::RIGHT);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		app->cam.processMove(dy::Camera::DESCEND);
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		app->cam.processMove(dy::Camera::ASCEND);
	}

	app->UpdateWindowView();*/

	//app->keyboardSystem->PrepareAll();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		app->keyboardSystem->SetKeyPressed(GLFW_KEY_W);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		app->keyboardSystem->SetKeyPressed(GLFW_KEY_S);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		app->keyboardSystem->SetKeyPressed(GLFW_KEY_A);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		app->keyboardSystem->SetKeyPressed(GLFW_KEY_D);
	}
}

void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	//this raw pointer is just a reference to the current object not the owner of the object
	App* app = (App*)glfwGetWindowUserPointer(window);
	app->cam.processZoom(yoffset);
	//app->projection = glm::perspective(glm::radians(app->cam.fov), app->w / app->h, 0.1f, 100.0f);
	app->projection = glm::ortho(-app->w / app->h * glm::radians(app->cam.fov), app->w / app->h * glm::radians(app->cam.fov), -1.0f * glm::radians(app->cam.fov), 1.0f * glm::radians(app->cam.fov), 0.1f, 100.f);
	app->UpdateWindowProjection();
}