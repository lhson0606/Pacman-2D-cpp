#pragma once
class TilePositionComponent
{
public:
	int x;
	int y;

	TilePositionComponent(int x, int y) :x(x), y(y) {}

	TilePositionComponent() :x(0), y(0) {}

	~TilePositionComponent() = default;
};

