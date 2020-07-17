#include "stdafx.h"
#include "Texture.h"


Texture::Texture()
{
}

Texture::Texture(GLuint texture, int width, int height)
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
