#pragma once

#include <cstdint>

class TileComponent
{
public:
	uint32_t id;

	TileComponent() = default;

	TileComponent(uint32_t id);
};
