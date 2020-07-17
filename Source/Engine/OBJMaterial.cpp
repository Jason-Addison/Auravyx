#include "stdafx.h"
#include "OBJMaterial.h"


OBJMaterial::OBJMaterial()
{
	this->shine = 0.4;
	this->damper = 7;
}

OBJMaterial::OBJMaterial(Texture texture, int length)
{
	this->texture = texture;
	this->length = length * 3;
	this->shine = 0.01;
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
