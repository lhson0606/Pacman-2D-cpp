#include "TextureManager.h"
#include <exception>
#include <assert.h>
#include "dy/Log.h"
#include "dy/glutils.h"

void TextureManager::HardLoadTextures()
{
	LoadTexture(MAP, MAP_TEXTURE_PATH);
	LoadTexture(GHOST, GHOST_TEXTURE_PATH);
}

void TextureManager::LoadTexture(TextureType type, const std::string& path)
{
	try {
		uint textureID = loadPixelStyleTexture(path);
		textures[type] = std::make_shared<Texture>(textureID);
	}
	catch (std::exception e)
	{
		DyLogger::LogArg(DyLogger::LOG_ERROR, "Failed to load texture: %s", e.what());
		assert(false && "Do something.");
	}
}

std::shared_ptr<Texture> TextureManager::GetTexture(TextureType type)
{
	return textures[type];
}