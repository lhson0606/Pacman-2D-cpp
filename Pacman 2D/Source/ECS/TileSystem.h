#pragma once

#include "ECS/ECS.h"
#include "Render/shader.h"
#include <string>
#include <Map/Map.h>
#include <memory>
#include <dy/Log.h>
#include "ECS/TileComponent.h"
#include "ECS/TransformComponent.h"

class Coordinator;

class TileSystem:public System
{
	using TileData = std::map<std::tuple<int, int>, tson::Tile*>;
public:
	//hard load constants
	inline static std::string WALL_LAYER = "Wall";
	inline static std::string ENERGIZED_PELLET_LAYER = "EnergizedPellet";
	inline static std::string PELLET_LAYER = "Pellet";
	inline static std::string UP_BANNED_LAYER = "UpBanned";
	inline static std::string TUNNEL_LAYER = "Tunnel";
	inline static std::string POSITION_LAYER = "Position";
	inline static std::string UI_LAYER = "UI";

	TileSystem() = default;

	void SetCoordinator(std::shared_ptr<Coordinator> coordinator)
	{
		this->coordinator = coordinator;
	}

	void InitMap(std::shared_ptr<Map> map)
	{
		CleanTiles();

		tileSize = map->GetTileSize();

		for (auto& layer : map->data->getLayers())
		{
			if (layer.getName() == WALL_LAYER)
			{
				HandleWallLayer(layer.getTileData(), map->GetWidth(), map->GetHeight());
			}
		}
	}

	void Draw(Shader& shader)
	{
		static bool test = true;

		if (test)
		{
			test = false;
			//print out the tiles
			for (auto entity : entities)
			{
				auto& tile = coordinator->GetComponent<TileComponent>(entity);
				auto& transform = coordinator->GetComponent<TransformComponent>(entity);

				DyLogger::LogArg(DyLogger::LOG_INFO, "Tile: %d, %d, %d", tile.x, tile.y, tile.id);
			}
		}
	}

private:
	int tileSize = 0;
	std::shared_ptr<Coordinator> coordinator;

	void CleanTiles()
	{
		//make a copy of the entities
		auto temp = this->entities;
		for (auto entity : temp)
		{
			coordinator->DestroyEntity(entity);
		}
	}

	void HandleWallLayer(TileData tileData, int w, int h)
	{
		DyLogger::LogArg(DyLogger::LOG_INFO, "Handling wall layer\n");

		for (int x = 0; x < w; ++x)
		{
			for (int y = 0; y < h; ++y)
			{
				auto tile = tileData[{x, y}];

				if (tile)
				{
					auto entity = coordinator->CreateEntity();
					coordinator->AddComponent<TileComponent>(entity, { x, y, tile->getId()});
					coordinator->AddComponent<TransformComponent>(entity, { x * tileSize + 0.0f, y * tileSize + 0.0f, 0.0f});
				}
			}
		}
	}
};

