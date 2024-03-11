#pragma once

#include "tileson.hpp"
#include <string>
#include <memory>
#include <exception>
#include "dy/Log.h"

class App;

#include <stdint.h>
using uint8 = uint8_t;

class Map
{
public:
	Map()
	{
		path = nullptr;
		data = nullptr;
	}

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

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	int GetTileSize() const { return tileSize; }

	~Map() {};

private:
	const char* path;
	uint8 tileSize;
	uint8 width;
	uint8 height;
	float scale = 0.1f;
};

