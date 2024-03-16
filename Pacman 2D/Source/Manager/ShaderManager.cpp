#include "ShaderManager.h"
#include <exception>
#include "dy/Log.h"
#include <assert.h>

void ShaderManager::HardLoadShaders()
{
	LoadShaders(MAP, MAP_VERTEX_SHADER_PATH, MAP_FRAGMENT_SHADER_PATH);
	LoadShaders(GHOST, GHOST_VERTEX_SHADER_PATH, GHOST_FRAGMENT_SHADER_PATH);
	LoadShaders(DEBUG_PATH, DEBUG_PATH_VERTEX_SHADER_PATH, DEBUG_PATH_FRAGMENT_SHADER_PATH);
	LoadShaders(TARGET_TILE, TARGET_TILE_VERTEX_SHADER_PATH, TARGET_TILE_FRAGMENT_SHADER_PATH);
	LoadShaders(PACMAN, PACMAN_VERTEX_SHADER_PATH, PACMAN_FRAGMENT_SHADER_PATH);
}

void ShaderManager::LoadShaders(ShaderType type, std::string vertexPath, std::string fragmentPath)
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

std::shared_ptr<Shader> ShaderManager::GetShader(ShaderType type)
{
	return shaders[type];
}