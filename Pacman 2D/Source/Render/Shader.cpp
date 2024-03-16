#include "Shader.h"
#include <glad/glad.h>
#include "dy/glutils.h"
#include <exception>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <assert.h>
#include "dy/Log.h"

using uint = unsigned int;

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	this->vertCode = readShader(vertexPath);
	this->fragPath = readShader(fragmentPath);
}

Shader::~Shader()
{
	//simply delete our shader program if it exists
	if (id != 0)
	{
		glDeleteProgram(id);
	}
	
	id = 0;
}

void Shader::Init()
{
	int compiledResult;
	char* infoLog = new char[SHADER_ERROR_LOG_SIZE+1];
	std::stringstream errMsgBuilder;

	//create vertex shader
	//if exception is thrown, it means that gl context is not created yet
	uint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderSourcePtr = vertCode.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderSourcePtr, NULL);
	glCompileShader(vertexShader);

	//check for errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiledResult);

	if (!compiledResult)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		errMsgBuilder << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		delete[] infoLog;
		std::cout << "Error: vertex shader compilation failed\n";
		std::cout << errMsgBuilder.str() << std::endl;
		throw std::exception(errMsgBuilder.str().c_str());
	}

	//create fragment shader
	uint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderSourcePtr = fragPath.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSourcePtr, NULL);
	glCompileShader(fragmentShader);

	//check for errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiledResult);

	if (!compiledResult)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		errMsgBuilder << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		delete[] infoLog;
		//delete vertex shader
		glDeleteShader(vertexShader);
		std::cout << "Error: fragment shader compilation failed\n";
		std::cout<< errMsgBuilder.str() << std::endl;
		throw std::exception(errMsgBuilder.str().c_str());
	}

	//create shader program
	id = glCreateProgram();

	assert(id != 0 && "Failed to create shader program, probably GL Context is not created!");

	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);

	//check for linking errors
	int linkedResult;
	glGetProgramiv(id, GL_LINK_STATUS, &linkedResult);

	if (!linkedResult)
	{
		glGetProgramInfoLog(id, SHADER_ERROR_LOG_SIZE, NULL, infoLog);
		errMsgBuilder << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		delete[] infoLog;
		//delete vertex and fragment shaders
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		throw std::exception(errMsgBuilder.str().c_str());
	}

	delete[] infoLog;
	//we don't need the shaders anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Use()
{
	glUseProgram(id);
}

void Shader::Stop()
{
	glUseProgram(0);
}

void Shader::SetBool(const std::string& name, bool value)
{
	int loc = GetUniLocation(name);
	glUniform1i(loc, (int)value);
}

void Shader::SetInt(const std::string& name, int value)
{
	int loc = GetUniLocation(name);
	glUniform1i(loc, value);
}

void Shader::SetFloat(const std::string& name, float value)
{
	int loc = GetUniLocation(name);
	glUniform1f(loc, value);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value)
{
	int loc = GetUniLocation(name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value)
{
	int loc = GetUniLocation(name);
	glUniform3f(loc, value.x, value.y, value.z);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z)
{
	int loc = GetUniLocation(name);
	glUniform3f(loc, x, y, z);
}

void Shader::ResetCache()
{
	uniformLocationCache.clear();
}

int Shader::GetUniLocation(const std::string& name)
{
	if (uniformLocationCache.find(name) == uniformLocationCache.end())
	{
		int res = glGetUniformLocation(id, name.c_str());

		if (res == -1)
		{
			//pretty funny, but it can cause lots of trouble :))
			//see: https://www.youtube.com/live/znJZsnTDXEk?si=Er5Wv8aVaa3f1Y7q&t=8114
			DyLogger::LogArg(DyLogger::LOG_ERROR, "Error: uniform \"%s\" not found or it's never used!", name.c_str());
			throw std::exception("uniform not found");
		}

		uniformLocationCache[name] = res;
	}

	return uniformLocationCache[name];
}
