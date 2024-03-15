#include "dy/Log.h"

#include <iostream>
#include <assert.h>
#include <Windows.h>

void DyLogger::Log(const char* message)
{
	//print time first
	//get the current time
	SetConsoleColor(CONSOLE_GRAY_COLOR);
	time_t now = time(0);
	//convert now to string form
	char buffer[26];
	ctime_s(buffer, 26, &now);
	//print the time
	std::cout << buffer;

	//print the message
	SetConsoleColor(CONSOLE_WHITE_COLOR);
	std::cout << "[INFO]: " << message;

	//reset the color to white
	ResetConsoleColor();
	std::cout << "\n";
}

void DyLogger::Log(const std::string& message)
{
	//print time first
	//get the current time
	SetConsoleColor(CONSOLE_GRAY_COLOR);
	time_t now = time(0);
	//convert now to string form
	char buffer[26];
	ctime_s(buffer, 26, &now);
	//print the time
	std::cout << buffer;

	//print the message
	SetConsoleColor(CONSOLE_WHITE_COLOR);
	std::cout << "[INFO]: " << message;

	//reset the color to white
	ResetConsoleColor();
	std::cout << "\n";
}

void DyLogger::Log(LogType type, const char* message)
{
	//print time first
	//get the current time
	SetConsoleColor(CONSOLE_GRAY_COLOR);
	time_t now = time(0);
	//convert now to string form
	char buffer[26];
	ctime_s(buffer, 26, &now);
	//print the time
	std::cout << buffer;

	switch (type)
	{
	case DyLogger::LOG_INFO:
		SetConsoleColor(CONSOLE_WHITE_COLOR);
		std::cout << "[INFO]: " << message;
		break;
	case DyLogger::LOG_WARNING:
		SetConsoleColor(CONSOLE_YELLOW_COLOR);
		std::cout << "[WARNING]: " << message;
		break;
	case DyLogger::LOG_ERROR:
		SetConsoleColor(CONSOLE_RED_COLOR);
		std::cout << "[ERROR]: " << message;
		break;
	default:
		assert(false && "Not implemented");
		break;
	}

	//reset the color to white
	ResetConsoleColor();
	std::cout << "\n";
}

void DyLogger::LogArg(LogType type, const char* format, ...)
{
	//print time first
	//get the current time
	SetConsoleColor(CONSOLE_GRAY_COLOR);
	time_t now = time(0);
	//convert now to string form
	char buffer[26];
	ctime_s(buffer, 26, &now);
	//print the time
	std::cout << buffer;

	switch (type)
	{
	case DyLogger::LOG_INFO:
		SetConsoleColor(CONSOLE_WHITE_COLOR);
		std::cout << "[INFO]: ";
		break;
	case DyLogger::LOG_WARNING:
		SetConsoleColor(CONSOLE_YELLOW_COLOR);
		std::cout << "[WARNING]: ";
		break;
	case DyLogger::LOG_ERROR:
		SetConsoleColor(CONSOLE_RED_COLOR);
		std::cout << "[ERROR]: ";
		break;
	default:
		break;
	}

	//get the variable arguments
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	//reset the color to white
	ResetConsoleColor();
	std::cout << "\n";
}

void DyLogger::ResetConsoleColor()
{
	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_WHITE_COLOR);
}

void DyLogger::SetConsoleColor(int color)
{
	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}