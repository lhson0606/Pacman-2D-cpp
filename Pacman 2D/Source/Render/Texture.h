#pragma once

#include "dy/dyutils.h"
#include "dy/glutils.h"
#include <string>

class Texture
{
public:
	Texture()
	{
		id = 0;
	}

	Texture(uint id)
	{
		this->id = id;
	}

	void Attach(uint slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void Detach()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	~Texture()
	{
		glDeleteTextures(1, &id);
	}
private:
	uint id;
};

