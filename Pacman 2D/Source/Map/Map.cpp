#include "Map.h"
#include <sstream>

Map::Map()
{
	path = nullptr;
	data = nullptr;
}

int Map::GetWidth() const { return width; }
int Map::GetHeight() const { return height; }
int Map::GetTileSize() const { return tileSize; }

void Map::InitWallLookUp()
{
	auto wallLayer = data->getLayer(Map::WALL_LAYER);
	auto wallData = wallLayer->getTileData();

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			auto tile = wallData[{i, j}];

			if (tile != nullptr)
			{
				if (tile->getId() != 0)
				{
					wallLookUp.set(i + j * width);
				}
			}
		}
	}
}

bool Map::IsWall(int x, int y) const
{
	return wallLookUp[x + y * width];
}

bool Map::IsMoveableByGhost(int x, int y) const
{
	return !IsWall(x, y);
}

std::string Map::GetDebugString() const
{
	std::stringstream ss;
	ss << "\n=====Wall Look Up=====\n";

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			if(IsWall(i, j))
			{
				ss << "# ";
			}
			else
			{
				ss << ". ";
			}
		}
		ss << "\n";
	}

	ss << "\n====================\n";

	return ss.str();
}
