#pragma once

#include "dy/dyutils.h"
#include <string>

class Texture
{
public:
	Texture();

	Texture(uint id);

	void Attach(uint slot);

	void Detach();

	~Texture();
private:
	uint id;
	int width = 0;
	int height = 0;
};
