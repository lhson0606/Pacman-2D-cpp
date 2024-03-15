#pragma once

#include <string>

class DyLogger
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

	static void Log(const char* message);
	static void Log(const std::string& message);
	static void Log(LogType type, const char* message);
	static void LogArg(LogType type, const char* format, ...);
	static void LogArg(const char* format, ...);
	static void ResetConsoleColor();
	static void SetConsoleColor(int color);
};