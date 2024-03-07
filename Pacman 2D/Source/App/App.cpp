#include "App.h"

#include <iostream>

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
	
	while (!glfwWindowShouldClose(window) && this->isRunning)
	{
		//process any input events
		processInput(window);

		//our rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		Draw();
		Update(0.0f);

		//swap buffer for the next frame to be drawn
		glfwSwapBuffers(window);
		//check for any events to happen (keyboard, mouse movement, etc)
		glfwPollEvents();
	}

	return 0;
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

	return 0;
}

void App::Draw()
{
}

void App::Update(float dt)
{
}

App::~App()
{
	//release glfw resources before exiting
	glfwTerminate();
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	static float lastX = 0.0f;
	static float lastY = 0.0f;
	static bool firstMouse = true;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float dx = xpos - lastX;
	float dy = ypos - lastY;

	lastX = xpos;
	lastY = ypos;

	//cam.processMouseInput(dx, -dy);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		//TODO
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		//TODO
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		//TODO
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		//TODO
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		//TODO
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		//TODO
	}
}

void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	//TODO
}
