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
		GHOST,
	};

	inline const static std::string MAP_VERTEX_SHADER_PATH = "Resources/GLSL/map.vert";
	inline const static std::string MAP_FRAGMENT_SHADER_PATH = "Resources/GLSL/map.frag";
	inline const static std::string GHOST_VERTEX_SHADER_PATH = "Resources/GLSL/ghost.vert";
	inline const static std::string GHOST_FRAGMENT_SHADER_PATH = "Resources/GLSL/ghost.frag";

	void HardLoadShaders()
	{
		LoadShaders(MAP, MAP_VERTEX_SHADER_PATH, MAP_FRAGMENT_SHADER_PATH);
		LoadShaders(GHOST, GHOST_VERTEX_SHADER_PATH, GHOST_FRAGMENT_SHADER_PATH);
	}

	void LoadShaders(ShaderType type, std::string vertexPath, std::string fragmentPath)
	{
		try {
			shaders[type] = std::make_shared<Shader>(vertexPath, fragmentPath);
			shaders[type]->Init();
		}
		catch (std::exception& e)
		{
			DyLogger::LogArg(DyLogger::LOG_ERROR, "Error when loading shader: %s and %s", vertexPath.c_str(), fragmentPath.c_str());
			DyLogger::LogArg(DyLogger::LOG_ERROR, "Failed to load shader: %s", e.what());
			assert(false && "Do something.");
		}
		
	}

	std::shared_ptr<Shader> GetShader(ShaderType type)
	{
		return shaders[type];
	}

	~ShaderManager()
	{
	}
private:
	std::map<ShaderType, std::shared_ptr<Shader>> shaders;
};

