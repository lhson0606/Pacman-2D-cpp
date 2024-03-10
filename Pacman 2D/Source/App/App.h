#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

class App
{
public:
	enum LogType
	{
		LOG_INFO, LOG_WARNING, LOG_ERROR
	};

	inline static int CONSOLE_WHITE_COLOR = 15;
	inline static int CONSOLE_GRAY_COLOR = 8;
	inline static int CONSOLE_RED_COLOR = 12;
	inline static int CONSOLE_YELLOW_COLOR = 14;

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
	void OnCreate();
	void Draw();
	void Update(float dt);
	void OnClose();

	void Log(LogType type, const char* message);
	void LogArg(LogType type, const char* format, ...);
	void ResetConsoleColor();
	void SetConsoleColor(int color);
};
