#pragma once

#include "Render/Shader.h"
#include <map>
#include <exception>
#include "dy/Log.h"
#include <assert.h>
#include <string>

class ShaderManager
{
public:
	enum ShaderType
	{
		MAP,
	};

	inline const static std::string MAP_VERTEX_SHADER_PATH = "Resources/GLSL/map.vert";
	inline const static std::string MAP_FRAGMENT_SHADER_PATH = "Resources/GLSL/map.frag";

	void HardLoadShaders()
	{
		LoadShaders(MAP, MAP_VERTEX_SHADER_PATH, MAP_FRAGMENT_SHADER_PATH);
	}

	void LoadShaders(ShaderType type, std::string vertexPath, std::string fragmentPath)
	{
		try {
			shaders[type] = Shader(vertexPath, fragmentPath);
			shaders[type].Init();
		}
		catch (std::exception& e)
		{
			DyLogger::LogArg(DyLogger::LOG_ERROR, "Failed to load shader: %s", e.what());
			assert(false && "Do something.");
		}
		
	}

	Shader& GetShader(ShaderType type)
	{
		return shaders[type];
	}

	~ShaderManager()
	{
	}
private:
	std::map<ShaderType, Shader> shaders;
};

