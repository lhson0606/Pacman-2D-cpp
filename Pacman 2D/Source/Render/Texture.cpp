#include "Texture.h"
#include <glad/glad.h>

Texture::Texture()
{
	id = 0;
}

Texture::Texture(uint id)
{
	this->id = id;
}

void Texture::Attach(uint slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Detach()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}