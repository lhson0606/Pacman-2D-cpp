#pragma once

#include "Render/Shader.h"
#include <map>

class ShaderManager
{
public:
	enum ShaderType
	{
		MAP,
	};

	inline static std::string MAP_VERTEX_SHADER_PATH = "Resources/Shaders/map.vert";
	inline static std::string MAP_FRAGMENT_SHADER_PATH = "Resources/Shaders/map.frag";

	void HardLoadShaders()
	{
		LoadShaders(MAP, MAP_VERTEX_SHADER_PATH.c_str(), MAP_FRAGMENT_SHADER_PATH.c_str());
	}

	void LoadShaders(ShaderType type, const char* vertexPath, const char* fragmentPath)
	{
		shaders[type] = Shader(vertexPath, fragmentPath);
		shaders[type].Init();
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

