#pragma once

#include "Render/Texture.h"
#include <memory>

class SpriteComponent
{
public:
private:
	std::shared_ptr<Texture> texture;
	float rowStride = 0;
	float colStride = 0;
	int rowIdx = 0;
	int colIdx = 0;
};
