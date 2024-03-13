#pragma once

#include "tileson.hpp"
#include <string>
#include <memory>
#include <exception>
#include "dy/Log.h"

#include <stdint.h>
using uint8 = uint8_t;

class Map
{
public:

	//hard load constants
	inline static const std::string WALL_LAYER = "Wall";
	inline static const std::string ENERGIZED_PELLET_LAYER = "EnergizedPellet";
	inline static const std::string PELLET_LAYER = "Pellet";
	inline static const std::string UP_BANNED_LAYER = "UpBanned";
	inline static const std::string TUNNEL_LAYER = "Tunnel";
	inline static const std::string POSITION_LAYER = "Position";
	inline static const std::string UI_LAYER = "UI";

	inline static const std::string BLINKY_START_POS_NAME = "RedStart";
	inline static const std::string PINKY_START_POS_NAME = "PinkyStart";
	inline static const std::string INKY_START_POS_NAME = "LimeStart";
	inline static const std::string CLYDE_START_POS_NAME = "OrangeStart";

	inline static const std::string PLAYER_START_POS_NAME = "PlayerStart";
	inline static const std::string GHOST_HOUSE_INSIDE_POS_NAME = "GhostHouseInside";
	inline static const std::string GHOST_HOUSE_OUTSIDE_POS_NAME = "GhostHouseOutside";

	Map();

	std::unique_ptr<tson::Map> data;

	/*
	* Load a map from a file
	* @param path: the path to the map file
	* @return: a unique pointer to the map
	* @throw: std::exception if the map fails to load
	*/
	inline static std::shared_ptr<Map> LoadMap(const char* path) noexcept(false)
	{
		tson::Tileson t;
		std::shared_ptr<Map> map = std::make_shared<Map>();
		map->path = path;
		map->data = t.parse(path);

		if (map->data->getStatus() == tson::ParseStatus::OK)
		{
			DyLogger::LogArg(DyLogger::LOG_INFO, "Map loaded successfully: %s\n", path);
		}
		else
		{
			DyLogger::LogArg(DyLogger::LOG_ERROR, "Failed to load map: %s\n", path);
			throw std::exception("Failed to load map");
		}

		assert(!map->data->isInfinite() && "Not supported");
		assert(map->data->getTileSize().x == map->data->getTileSize().y && "Tile size must be square");

		map->tileSize = map->data->getTileSize().x;
		map->width = map->data->getSize().x;
		map->height = map->data->getSize().y;

		return map;
	}

	int GetWidth() const;
	int GetHeight() const;
	int GetTileSize() const;

	~Map() {};

private:
	const char* path;
	uint8 tileSize;
	uint8 width;
	uint8 height;
	float scale = 0.1f;
};
