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
		PACMAN,
		DEBUG_PATH,
		TARGET_TILE,
	};

	inline const static std::string MAP_VERTEX_SHADER_PATH = "Resources/GLSL/map.vert";
	inline const static std::string MAP_FRAGMENT_SHADER_PATH = "Resources/GLSL/map.frag";
	inline const static std::string GHOST_VERTEX_SHADER_PATH = "Resources/GLSL/ghost.vert";
	inline const static std::string GHOST_FRAGMENT_SHADER_PATH = "Resources/GLSL/ghost.frag";
	inline const static std::string DEBUG_PATH_VERTEX_SHADER_PATH = "Resources/GLSL/path.vert";
	inline const static std::string DEBUG_PATH_FRAGMENT_SHADER_PATH = "Resources/GLSL/path.frag";
	inline const static std::string TARGET_TILE_FRAGMENT_SHADER_PATH = "Resources/GLSL/target_tile.frag";
	inline const static std::string TARGET_TILE_VERTEX_SHADER_PATH = "Resources/GLSL/target_tile.vert";
	inline const static std::string PACMAN_VERTEX_SHADER_PATH = "Resources/GLSL/pacman.vert";
	inline const static std::string PACMAN_FRAGMENT_SHADER_PATH = "Resources/GLSL/pacman.frag";

	void HardLoadShaders();

	void LoadShaders(ShaderType type, std::string vertexPath, std::string fragmentPath);

	std::shared_ptr<Shader> GetShader(ShaderType type);

private:
	std::map<ShaderType, std::shared_ptr<Shader>> shaders;
};
