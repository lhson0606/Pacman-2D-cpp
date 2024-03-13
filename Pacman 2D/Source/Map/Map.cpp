#include "Map.h"

Map::Map()
{
	path = nullptr;
	data = nullptr;
}

int Map::GetWidth() const { return width; }
int Map::GetHeight() const { return height; }
int Map::GetTileSize() const { return tileSize; }