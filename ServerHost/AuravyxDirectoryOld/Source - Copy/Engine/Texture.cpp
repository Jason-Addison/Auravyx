#include "stdafx.h"
#include "Engine/Texture.h"


Texture::Texture()
{
}

Texture::Texture(const GLuint texture, const int width, const int height)
{
	this->texture = texture;
	this->width = width;
	this->height = height;
}

Texture::~Texture()
{
}

GLuint Texture::getTexture()
{
	return texture;
}

int Texture::getWidth()
{
	return width;
}

int Texture::getHeight()
{
	return height;
}

void Texture::destroy()
{
	glDeleteTextures(1, &texture);
}
