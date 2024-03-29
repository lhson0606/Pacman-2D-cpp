#pragma once

#include "Render/Texture.h"
#include <map>
#include <string>
#include <memory>

class TextureManager
{
public:
	enum TextureType
	{
		MAP,
		GHOST,
		PACMAN,
		TARGET_TILE,
	};

	inline const static std::string MAP_TEXTURE_PATH = "Resources/Textures/pacman_map.png";
	inline const static std::string GHOST_TEXTURE_PATH = "Resources/Textures/ghost.png";
	inline const static std::string TARGET_TILE_PATH = "Resources/Textures/target_tile.png";
	inline const static std::string PACMAN_TEXTURE_PATH = "Resources/Textures/pacman.png";

	void HardLoadTextures();

	void LoadTexture(TextureType type, const std::string& path);

	std::shared_ptr<Texture> GetTexture(TextureType type);

private:
	std::map<TextureType, std::shared_ptr<Texture>> textures;
};
