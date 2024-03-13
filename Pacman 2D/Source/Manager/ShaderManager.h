#pragma once

#include "Render/Shader.h"
#include <map>
#include <string>

class ShaderManager
{
public:
	enum ShaderType
	{
		MAP,
		GHOST,
		DEBUG_PATH,
	};

	inline const static std::string MAP_VERTEX_SHADER_PATH = "Resources/GLSL/map.vert";
	inline const static std::string MAP_FRAGMENT_SHADER_PATH = "Resources/GLSL/map.frag";
	inline const static std::string GHOST_VERTEX_SHADER_PATH = "Resources/GLSL/ghost.vert";
	inline const static std::string GHOST_FRAGMENT_SHADER_PATH = "Resources/GLSL/ghost.frag";
	inline const static std::string DEBUG_PATH_VERTEX_SHADER_PATH = "Resources/GLSL/path.vert";
	inline const static std::string DEBUG_PATH_FRAGMENT_SHADER_PATH = "Resources/GLSL/path.frag";

	void HardLoadShaders();

	void LoadShaders(ShaderType type, std::string vertexPath, std::string fragmentPath);

	std::shared_ptr<Shader> GetShader(ShaderType type);

private:
	std::map<ShaderType, std::shared_ptr<Shader>> shaders;
};
