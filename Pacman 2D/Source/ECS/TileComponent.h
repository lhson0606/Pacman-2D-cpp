#pragma once

#include <cstdint>

class TileComponent
{
public:
	int x;
	int y;
	uint32_t id;

	TileComponent() = default;

	TileComponent(int x, int y, uint32_t id)
	{
		this->x = x;
		this->y = y;
		this->id = id;
	}
};

