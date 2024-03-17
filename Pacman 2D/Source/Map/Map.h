#pragma once

#include "tileson.hpp"
#include <string>
#include <memory>
#include <exception>
#include "dy/Log.h"
#include <stdint.h>
#include <bitset>

using uint8 = uint8_t;

#define MAP_MAX_TILE_COUNT 2500 //50x50
#define TILE_OFFSET 0.5f

class Map
{
public:

	//hard load constants in tiled layers
	inline static const std::string WALL_LAYER = "Wall";
	inline static const std::string ENERGIZED_PELLET_LAYER = "EnergizedPellet";
	inline static const std::string PELLET_LAYER = "Pellet";
	inline static const std::string UP_BANNED_LAYER = "UpBanned";
	inline static const std::string TUNNEL_LAYER = "Tunnel";
	inline static const std::string POSITION_LAYER = "Position";
	inline static const std::string UI_LAYER = "UI";

	//Blinky (red), Pinky (pink), Inky (cyan), and Clyde (orange)
	inline static const std::string BLINKY_NAME = "Blinky";
	inline static const std::string PINKY_NAME = "Pinky";
	inline static const std::string INKY_NAME = "Inky";
	inline static const std::string CLYDE_NAME = "Clyde";

	inline static const std::string BLINKY_START_POS_NAME = BLINKY_NAME + "Start";
	inline static const std::string PINKY_START_POS_NAME = PINKY_NAME + "Start";
	inline static const std::string INKY_START_POS_NAME = INKY_NAME + "Start";
	inline static const std::string CLYDE_START_POS_NAME = CLYDE_NAME + "Start";

	inline static const std::string PLAYER_START_POS_NAME = "PlayerStart";
	inline static const std::string GHOST_HOUSE_INSIDE_POS_NAME = "GhostHouseInside";
	inline static const std::string GHOST_HOUSE_OUTSIDE_POS_NAME = "GhostHouseOutside";

	inline static const std::string PINKY_SCATTER_POS_NAME = PINKY_NAME + "Scatter";
	inline static const std::string INKY_SCATTER_POS_NAME = INKY_NAME + "Scatter";
	inline static const std::string CLYDE_SCATTER_POS_NAME = CLYDE_NAME + "Scatter";
	inline static const std::string BLINKY_SCATTER_POS_NAME = BLINKY_NAME + "Scatter";

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

		assert(map->data->getTileSize().x == map->data->getTileSize().y && "Tile must be square");

		map->width = map->data->getSize().x;
		map->height = map->data->getSize().y;

		map->InitWallLookUp();

		return map;
	}

	int GetWidth() const;
	int GetHeight() const;
	int GetTileSize() const;
	void InitWallLookUp();
	bool IsWall(int x, int y) const;
	bool IsMoveableByGhost(int x, int y) const;
	std::string GetDebugString() const;

	~Map() {};

private:
	const char* path;
	uint8 tileSize;
	uint8 width;
	uint8 height;
	float scale = 0.1f;
	std::bitset<MAP_MAX_TILE_COUNT> wallLookUp{ 0 };
};
