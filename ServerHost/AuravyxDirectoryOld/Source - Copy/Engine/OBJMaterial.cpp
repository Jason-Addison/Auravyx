#include "stdafx.h"
#include "Engine/OBJMaterial.h"

OBJMaterial::OBJMaterial()
{
	this->shine = 0.4f;
	this->damper = 7;
	this->length = 1;
}

OBJMaterial::OBJMaterial(const Texture& texture, const int length)
{
	this->texture = texture;
	this->length = length * 3;
	this->shine = 0.01f;
	this->damper = 1;
}

OBJMaterial::~OBJMaterial()
{
}

int OBJMaterial::getLength()
{
	return length;
}
Texture OBJMaterial::getTexture()
{
	return texture;
}