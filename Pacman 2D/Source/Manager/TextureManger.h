#pragma once

#include "Render/Texture.h"
#include <map>
#include <exception>
#include <assert.h>
#include <string>
#include <memory>
#include "dy/Log.h"

class TextureManager
{
public:
	enum TextureType
	{
		MAP,
	};

	inline const static std::string MAP_TEXTURE_PATH = "Resources/Textures/pacman_map.png";

	void HardLoadTextures()
	{
		LoadTexture(MAP, MAP_TEXTURE_PATH);
	}

	void LoadTexture(TextureType type, const std::string& path)
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

	std::shared_ptr<Texture> GetTexture(TextureType type)
	{
		return textures[type];
	}

private:
	std::map<TextureType, std::shared_ptr<Texture>> textures;
};

